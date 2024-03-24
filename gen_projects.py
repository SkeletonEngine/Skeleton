import subprocess, platform

if platform.system() == 'Windows':
  subprocess.run(['cmake', '-B build/Windows'])

elif platform.system() == 'Darwin':
  subprocess.run(['cmake', '-B build/macOS', '-G Xcode'])
