# Naming conventions <!-- omit in toc -->

[Home](../README.md)

- [1. Variables name](#1-variables-name)
- [2. Functions name](#2-functions-name)
- [3. Classes](#3-classes)
- [4. Traces / Logs](#4-traces--logs)
- [5. Read](#5-read)

## 1. Variables name

- use camelCase for Variable Name (read case style at end of file)
```c++
// Example
int varName;
bool bIsEven;
```

- constants: kVarName  
- pointer: pVarName  
- boolean: bInit  

## 2. Functions name

- use snake_case for Function Name (read case style at end of file)
```c++
// Example
int process_new_data();
bool get_value();
```

- Use _ to describe action sentence  
-> Examples: HLT_make_socket_non_blocking, HLT_process_new_data, etc...  

## 3. Classes

- add m_ prefix to member variables  

## 4. Traces / Logs

- Prefix with "Error: ...", "Debug: ...", "Info: ..."  

```bash
# Examples
"Error: Initialization failed"
"Debug: Calling ..."
"Info : Cleaning resources"  
```

## 5. Read

- [Case Style](https://medium.com/better-programming/string-case-styles-camel-pascal-snake-and-kebab-case-981407998841)