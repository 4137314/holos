/* src/ui/gui/gui.c
 *
 * Minimal GTK3 GUI for Holos: lists registered physics functions and runs them.
 *
 * Build:
 *   gcc `pkg-config --cflags gtk+-3.0` -o holos-gui gui.c registry.c -Wl,--no-as-needed `pkg-config --libs gtk+-3.0`
 *
 * Note:
 * - This expects a registry.h providing:
 *     struct holos_entry { const char *name; void (*func)(void); };
 *     extern struct holos_entry holos_registry[];
 *     extern int holos_registry_count;
 *
 * - If you don't have a generated registry.c/h yet, create a small stub to test.
 */

#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

/* Include registry produced by autogen-registry.sh */
#include "registry.h" /* your project should provide this */

/* Column IDs for GtkListStore */
enum {
    COL_NAME = 0,
    COL_INDEX,
    N_COLUMNS
};

/* Helper: append text to GtkTextView (log area) */
static void log_append(GtkTextView *tv, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    GtkTextBuffer *buf = gtk_text_view_get_buffer(tv);
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(buf, &end);

    char *msg = NULL;
    if (vasprintf(&msg, fmt, ap) != -1 && msg) {
        gtk_text_buffer_insert(buf, &end, msg, -1);
        gtk_text_buffer_insert(buf, &end, "\n", -1);
        free(msg);
    }

    va_end(ap);

    /* auto-scroll */
    GtkAdjustment *adj = gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(tv));
    if (adj) {
        gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
    }
}

/* Run the function at index 'idx' and log result */
static void run_function_by_index(GtkTextView *log_view, int idx)
{
    if (idx < 0 || idx >= holos_registry_count) {
        log_append(log_view, "Invalid function index: %d", idx);
        return;
    }

    const char *name = holos_registry[idx].name ? holos_registry[idx].name : "<unnamed>";
    void (*f)(void) = holos_registry[idx].func;

    if (!f) {
        log_append(log_view, "Function '%s' has no implementation.", name);
        return;
    }

    log_append(log_view, "Running: %s", name);

    /* Run function. We don't capture stdout/stderr here; functions should log via API if needed. */
    /* For safety, run in try/catch? C has no exceptions — assume functions are well-behaved. */
    f();

    log_append(log_view, "Finished: %s", name);
}

/* Callback for Run button */
static void on_run_clicked(GtkButton *btn, gpointer user_data)
{
    GtkTreeView *tree = GTK_TREE_VIEW(user_data);
    GtkTreeSelection *sel = gtk_tree_view_get_selection(tree);
    GtkTreeModel *model;
    GtkTreeIter iter;
    if (gtk_tree_selection_get_selected(sel, &model, &iter)) {
        int index;
        gtk_tree_model_get(model, &iter, COL_INDEX, &index, -1);

        /* find the log view via widget name lookup from tree's toplevel */
        GtkWidget *toplevel = gtk_widget_get_toplevel(GTK_WIDGET(tree));
        GtkWidget *log = g_object_get_data(G_OBJECT(toplevel), "log_view");
        if (log && GTK_IS_TEXT_VIEW(log)) {
            run_function_by_index(GTK_TEXT_VIEW(log), index);
        }
    }
}

/* Filter: hide rows that don't match search */
static gboolean filter_func(GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
    const char *pattern = data;
    if (!pattern || pattern[0] == '\0') return TRUE;

    gchar *name;
    gtk_tree_model_get(model, iter, COL_NAME, &name, -1);
    gboolean match = FALSE;
    if (name) {
        /* case-insensitive substring match */
        gchar *lower_name = g_ascii_strdown(name, -1);
        gchar *lower_pat = g_ascii_strdown(pattern, -1);
        if (strstr(lower_name, lower_pat) != NULL) match = TRUE;
        g_free(lower_name);
        g_free(lower_pat);
        g_free(name);
    }
    return match;
}

/* Update filter when search entry changed */
static void on_search_changed(GtkSearchEntry *entry, gpointer user_data)
{
    GtkTreeModelFilter *filter = GTK_TREE_MODEL_FILTER(user_data);
    const gchar *txt = gtk_entry_get_text(GTK_ENTRY(entry));
    /* set user_data for filter callback by replacing its user_data via g_object_set_data */
    g_object_set_data(G_OBJECT(filter), "filter_pattern", g_strdup(txt));
    gtk_tree_model_filter_refilter(filter);
}

/* Adapter for GtkTreeModelFilter that reads pattern from object data */
static gboolean filter_wrapper(GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
    GtkTreeModelFilter *filter = GTK_TREE_MODEL_FILTER(user_data);
    const char *pattern = g_object_get_data(G_OBJECT(filter), "filter_pattern");
    return filter_func(model, iter, (gpointer)pattern);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    /* Top-level window */
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 480);
    gtk_window_set_title(GTK_WINDOW(window), "Holos - Physics GUI");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Main vertical box */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 8);

    /* Search entry */
    GtkWidget *search = gtk_search_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), search, FALSE, FALSE, 0);

    /* Horizontal paned area: left = list, right = log */
    GtkWidget *hpaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), hpaned, TRUE, TRUE, 0);

    /* Left: list store + tree view */
    GtkListStore *store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_INT);

    /* populate store from holos_registry */
    for (int i = 0; i < holos_registry_count; ++i) {
        GtkTreeIter iter;
        const char *name = holos_registry[i].name ? holos_registry[i].name : "<unnamed>";
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           COL_NAME, name,
                           COL_INDEX, i,
                           -1);
    }

    /* Filter model so we can refilter on search changes */
    GtkTreeModel *filter_model = GTK_TREE_MODEL(gtk_tree_model_filter_new(GTK_TREE_MODEL(store), NULL));
    /* store pointer to pattern in filter object; initially empty string */
    g_object_set_data(G_OBJECT(filter_model), "filter_pattern", g_strdup(""));

    /* Set custom filter function wrapper */
    gtk_tree_model_filter_set_visible_func(GTK_TREE_MODEL_FILTER(filter_model),
                                           filter_wrapper, filter_model, NULL);

    GtkWidget *tree = gtk_tree_view_new_with_model(filter_model);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree),
                                                -1, "Function", gtk_cell_renderer_text_new(),
                                                "text", COL_NAME, NULL);
    gtk_widget_set_vexpand(tree, TRUE);
    gtk_widget_set_hexpand(tree, TRUE);

    /* Put tree in scrolled window */
    GtkWidget *scroll_left = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll_left), tree);
    gtk_widget_set_size_request(scroll_left, 320, -1);
    gtk_paned_pack1(GTK_PANED(hpaned), scroll_left, TRUE, FALSE);

    /* Right: log area */
    GtkWidget *log_view = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(log_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(log_view), FALSE);
    gtk_widget_set_hexpand(log_view, TRUE);
    gtk_widget_set_vexpand(log_view, TRUE);

    GtkWidget *scroll_right = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll_right), log_view);
    gtk_paned_pack2(GTK_PANED(hpaned), scroll_right, TRUE, FALSE);

    /* Run button at bottom */
    GtkWidget *hbottom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_pack_start(GTK_BOX(vbox), hbottom, FALSE, FALSE, 0);

    GtkWidget *run_btn = gtk_button_new_with_label("Run");
    gtk_widget_set_sensitive(run_btn, TRUE);
    gtk_box_pack_start(GTK_BOX(hbottom), run_btn, FALSE, FALSE, 0);

    GtkWidget *help_lbl = gtk_label_new("Select a function and press Run");
    gtk_box_pack_start(GTK_BOX(hbottom), help_lbl, FALSE, FALSE, 0);

    /* store pointer to log_view in window object for callbacks */
    g_object_set_data(G_OBJECT(window), "log_view", log_view);

    /* Connect Run button */
    g_signal_connect(run_btn, "clicked", G_CALLBACK(on_run_clicked), tree);

    /* Connect search entry to filter */
    /* Need to keep a reference to GtkTreeModelFilter to pass into callback; we used GTK_TREE_MODEL(filter_model) above */
    GtkTreeModelFilter *tm_filter = GTK_TREE_MODEL_FILTER(filter_model);
    g_signal_connect(search, "search-changed", G_CALLBACK(on_search_changed), tm_filter);

    /* Also connect selection changed to update help label or enable/disable Run */
    GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
    gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

    /* Selection-changed callback to toggle run button sensitivity */
    g_signal_connect(sel, "changed", G_CALLBACK(
        +[](GtkTreeSelection *selection, gpointer data){
            GtkWidget *btn = GTK_WIDGET(data);
            GtkTreeModel *m;
            GtkTreeIter it;
            gboolean has = gtk_tree_selection_get_selected(selection, &m, &it);
            gtk_widget_set_sensitive(btn, has);
        }), run_btn);

    /* Show everything */
    gtk_widget_show_all(window);

    /* Initial log message */
    log_append(GTK_TEXT_VIEW(log_view), "Holos GUI started. %d functions available.", holos_registry_count);

    gtk_main();
    return 0;
}