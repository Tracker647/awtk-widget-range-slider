import os
import scripts.app_helper as app

helper = app.Helper(ARGUMENTS)
helper.set_dll_def('src/range_slider.def').set_libs(['range_slider']).call(DefaultEnvironment)

SConscriptFiles = ['src/SConscript', 'demos/SConscript', 'tests/SConscript']
helper.SConscript(SConscriptFiles)
