# Coding conventions <!-- omit in toc -->

[Home](../README.md)

- [1. Language](#1-language)
- [2. Review for each commit](#2-review-for-each-commit)
- [3. Coding Rules](#3-coding-rules)
  - [3.1. Return value](#31-return-value)
  - [3.2. Variable](#32-variable)
  - [3.3. Pointer](#33-pointer)
- [4. Divers](#4-divers)
  - [4.1. Logs](#41-logs)

## 1. Language
- Logs/traces must be in English  
- Comments must be in English  

## 2. Review for each commit
- Remove unnecessary space & tabulation changes in commits  

## 3. Coding Rules
### 3.1. Return value
- Respect following rules for integer return values in functions  

```
return <0 : KO - error code  
return 0  : OK - no error  
return >0 : OK with return value
```

### 3.2. Variable
- Always initialize variables

- Initialize variables at 0 and pointers at null if no other value set

```c++
GError *pError = NULL;
int iStatus = -1;
gsize length=0;
```


### 3.3. Pointer
- **Check if there is a deallocation needed for each pointer used**
- Check pointer is not null before accessing

```c++
if (Err != NULL) {
    cout <<"HLT_ERR : "<< Err->message<<endl;
}
```

## 4. Divers
### 4.1. Logs
- Use a standardized format for logs/traces  
- Use charset headers and functions in HLT_utils.h  

```bash
** HLT Debug: Initializing DBus connections and objects  
** HLT Info: Checking config file  
** HLT Debug: Start LED timer OK
```

- Manage errors and report to upper layer if not handled

- Restrict scope of variables according to their usage

  Ex: No class members variables used as local

- Choose very explicit function names

  Ex: print_and_free_error()

- Use preferably C++ containers

  http://www.cplusplus.com/reference/stl/  
  vector, set, map, etc....

- Respect naming conventions rules

  Find at [Naming rules](./Naming_rules.md)

- Use exceptions if relevant
