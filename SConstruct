from SConsStandard import EnvMod
import os 

env = EnvMod.getEnvironment()
env.withCompilationDB()

env.environment['ENV']['ASAN_OPTIONS'] = 'halt_on_error=0;detect_leaks=0'

# Run the executable submodule
exe = env.SConscript("src/SConscript", variant_dir="src", duplicate = 0)

