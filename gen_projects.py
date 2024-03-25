import subprocess, platform

subprocess.run(['git', 'submodule', 'update', '--init', '--remote', '--recursive'])

if platform.system() == 'Windows':
  subprocess.run(['cmake', '-B build/Windows'])

elif platform.system() == 'Darwin':
  subprocess.run(['cmake', '-B build/macOS', '-G Xcode'])
