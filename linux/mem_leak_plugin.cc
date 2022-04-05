#include "include/mem_leak/mem_leak_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>
#include <string>

#define MEM_LEAK_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), mem_leak_plugin_get_type(), \
                              MemLeakPlugin))

struct _MemLeakPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(MemLeakPlugin, mem_leak_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void mem_leak_plugin_handle_method_call(
    MemLeakPlugin* self,
    FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "getPlatformVersion") == 0) {
    struct utsname uname_data = {};
    uname(&uname_data);
    g_autofree gchar *version = g_strdup_printf("Linux %s", uname_data.version);
    g_autoptr(FlValue) result = fl_value_new_string(version);
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(result));
  } else if(strcmp(method, "test") == 0) {
    for(int i = 0; i<100000; ++i) {
      printf("%i\n", i);
      g_autoptr(FlValue) test = fl_value_new_map();
      fl_value_set_string(test, "test", fl_value_new_string("test"));
    }
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void mem_leak_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(mem_leak_plugin_parent_class)->dispose(object);
}

static void mem_leak_plugin_class_init(MemLeakPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = mem_leak_plugin_dispose;
}

static void mem_leak_plugin_init(MemLeakPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  MemLeakPlugin* plugin = MEM_LEAK_PLUGIN(user_data);
  mem_leak_plugin_handle_method_call(plugin, method_call);
}

void mem_leak_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  MemLeakPlugin* plugin = MEM_LEAK_PLUGIN(
      g_object_new(mem_leak_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "mem_leak",
                            FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb,
                                            g_object_ref(plugin),
                                            g_object_unref);

  g_object_unref(plugin);
}
