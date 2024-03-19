cw
==
A generic command wrapper

### TODOs
- [ ] use [`CreateProcessW`](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw)
- [ ] nested `.cw` script
- [ ] `.secret` file (as a replacement for hardcoded .bat / .sh)
- [ ] basic string functions
  - [ ] slash conversion
  - [ ] string part extraction
- [ ] built-in
    - IO
      - replace content (template file)
    - http curl client
    - multiple instance mutex
      - https://github.com/dirkarnez/cpp-mutex-playground
- [ ] child process stdin + stdout Redirection
- [ ] [ohmyzsh/ohmyzsh](https://github.com/ohmyzsh/ohmyzsh)
- [ ] [malxau/yori: Yori is a CMD replacement shell that supports backquotes, job control, and improves tab completion, file matching, aliases, command history, and more.](https://github.com/malxau/yori)
- [ ] [walterschell/Lua: CMake based build of Lua (5.4.3 and 5.3.3)](https://github.com/walterschell/Lua)
- [ ] GUI for command list with tags
- [ ] Repl mode
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
  - fuzzy search on `.cw` script contents and tags 
- [ ] Should support cli (for CICD environment)
- [ ] timeout
  - ```c++
    #include <iostream>
    #include <boost/process.hpp>
    #include <boost/asio.hpp>
    
    namespace bp = boost::process;
    
    int main() {
        boost::asio::io_context io_context;
        bp::child c("sleep 5", bp::std_out > stdout, bp::std_err > stderr, io_context);
    
        // Set a 5-second timeout
        io_context.run_for(std::chrono::seconds(5));
    
        // Check if the child process has finished
        if (c.running()) {
            // If the child process is still running after 5 seconds, terminate it
            c.terminate();
        }
    
        return 0;
    }
    ```
- [ ] cross-platform
  - [lipnitsk/llconf: Automatically exported from code.google.com/p/llconf](https://github.com/lipnitsk/llconf)
### Grammar
```actionscript
import default_env;

export const name: string = "";
export const (): error => {
  var hour: int = prompt("", 0);
  var minute: int = prompt("", 0);
  var second: int = prompt("", 0);

  synchronized download_movie(), download_music()

  return exec({
      ...default_env,
      "PATH": Path(C:/Windows/System32)
    },
    this_script_location, //working_directory
    "shutdown -s -f -t ${(hour * 60 * 60) + (minute * 60) + second}",
    5
  );
}

```
### Notes
- Should not touch `%PATH%`, including this software
  - pin to taskbar is ok

### Scripts
- [cw-scripts](https://github.com/dirkarnez/cw-scripts)
