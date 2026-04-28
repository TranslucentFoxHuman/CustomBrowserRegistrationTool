# CBRT - Custom web Browser Registration Tool for Windows

## What's this?
This program is a small tool to register custom command as Web browser to Windows.
By using this program, you can register any command as a web browser to Windows.

This program was developed in the wake of the fact that when I searched for a way to make arbitrary commands recognized as web browsers, the search results were contaminated on a lot of ridiculous websites that everyone knows "how to set a default browser".

## How to use
This program must be run with administrator privileges. Open the command prompt with administrator privileges first, and then run this program. Otherwise, you will not be able to see the output of the program.

```
Usage:
cbrt.exe </i ID> </n NAME> </c COMMAND> [/d DESCRIPTION] [/h | -h | --help] [/v]

Options:
/n              : Specify the name of the application.
/i              : Specify the ID of the application. You can specify an arbitrary string, but only half-width alphanumeric characters can be used, there should be no space and \.
/c              : Specify the command line to launch.
/d              : Specify the description of the browser entry. (Optional)
/v              : Print version information.
/h /? -h --help : Print this help message.
```

After you execute this command, the program with the specified name should appear in the default app settings item of the Windows settings app. By selecting it, you can set any arbitrary command as the default web browser.

The ID specified by the "/i" option can be any string of half-width alphanumeric characters. If the same ID already exists in the system, this program overrides it. Perhaps you should avoid IDs such as "Firefox" and "Chrome", and specify it considering the ID that is more difficult to cover.

On the command line specified by the /c option, double quotes must be escaped with "\". However, "\" alone does not need to escape.  
For example, let's say we want to specify the string `"C:\Program Files\Mozilla Firefox\firefox.exe"`. In this case, it will be as follows.
```
/c "\"C:\Program Files\Mozilla Firefox\firefox.exe\""
```

Also, don't forget to include `\"%1\"` on the command line you specify in the /c option!
%1 is replaced by arguments given by Windows. So if this is missing, the URL or file will not be opened, just the program will open.

## Usage Examples
- Register "C:\Program Files\MyBrowser\Main.exe" as a browser:  
```
cbrt.exe /i MyBrowser /n "My custom browser" /c "\"C:\Program Files\MyBrowser\Main.exe\" \"%1\"" /d 
"My self-made browser application"
```
- Register Firefox that opens in private window:  
```
cbrt.exe /i FirefoxPrivate /n "Firefox Private Window" /c "\"C:\Program Files\Mozilla Firefox\firefox.exe\" --private-window \"%1\""
```

## How to build
This program can be built with Microsoft Visual C++ or MinGW-w64.

For MSVC, open the MSVC's Developer Command Prompt first and run the following:
```
make-msvc.bat
```

For MinGW-w64, run the following:
```
mingw32-make
```

If it is built successfully, `cbrt.exe` should have been generated.

## License
This program is provided under the GNU General Public License Version 3 or later.