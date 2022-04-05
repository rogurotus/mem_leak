//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <mem_leak/mem_leak_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) mem_leak_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "MemLeakPlugin");
  mem_leak_plugin_register_with_registrar(mem_leak_registrar);
}
