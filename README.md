cw
==
A generic command wrapper

### TODOs
- [ ] nested `.cw` script
- [ ] basic string functions
  - [ ] slash conversion
  - [ ] string part extraction
- [ ] built-in IO
  - replace content (template file)
- [ ] child process stdin + stdout Redirection
- [ ] [ohmyzsh/ohmyzsh](https://github.com/ohmyzsh/ohmyzsh)
- [ ] [walterschell/Lua: CMake based build of Lua (5.4.3 and 5.3.3)](https://github.com/walterschell/Lua)
- [ ] GUI for command list with tags
- [ ] Variables
  - [**dirkarnez/string-substitutions-playground**](https://github.com/dirkarnez/string-substitutions-playground)
  - [adeharo9/cpp-dotenv: Loads environment variables from .env files for C++ projects.](https://github.com/adeharo9/cpp-dotenv#variable-resolution)
  - [**dirkarnez/boost-env-boilerplate**](https://github.com/dirkarnez/boost-env-boilerplate)
  - passing variable or return value between steps
  - session-like variable (like docker container id) for random access
  - conditional step (like github workflow)
    - prerequisites
- [ ] Profiles (e.g. for C++ development) 
  - environment settings with execution placeholder(s)?
- [ ] Autocompletion
  - say typing "docker" will show all things related to docker
  - `.cw` scripts and their usage are not meant to be memorized
### Notes
- Should not touch `%PATH%`, including this software
  - pin to taskbar is ok

### Scripts
- [cw-scripts](https://github.com/dirkarnez/cw-scripts)
