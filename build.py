import os, platform, subprocess

submodule_paths = [
  'src/lib/glfw',
  'src/lib/imgui/imgui'
]
any_submodule_dirs_empty = False
for path in submodule_paths:
  if len(os.listdir(path) ) == 0:
    any_submodule_dirs_empty = True
    break

if any_submodule_dirs_empty: 
  print("One or more submodule directories empty, pulling changes...") 
  subprocess.run(['git', 'submodule', 'update', '--init', '--recursive'])

if platform.system() == 'Darwin':
  print('macOS detected, building project files for Xcode...')
  subprocess.run(['cmake', '-B', 'build', '-G', 'Xcode'])
elif platform.system() == 'Windows':
  print('Windows detected, building project files for Visual Studio...')
  subprocess.run(['cmake', '-B', 'build'])
elif platform.system() == 'Linux':
  print('Linux detected, building Unix Makefiles...')
  subprocess.run(['cmake', '-B', 'build', '-G', 'Unix Makefiles'])
