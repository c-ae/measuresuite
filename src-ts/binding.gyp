{
  "targets": [
    {
      "target_name": "measuresuite",
      "sources": ["./measuresuite_binding.c"],
      # the first include if for measuresuite.h, second for <node/node_api.h>
      "include_dirs": ["<(module_root_dir)/include", "<(module_root_dir)/../../../bins/node/include"],
      "libraries": ["<(module_root_dir)/lib/libmeasuresuite.a", "-lassemblyline"]
    }
  ]
}
