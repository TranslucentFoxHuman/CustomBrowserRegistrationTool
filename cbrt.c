//このプログラムには日本語の文字列が含まれます。Text editors will recognize the Shift-JIS code by this line.
#define __PROG_VER__ "1.0.0"

/*
 * CBRT - Custom Browser Registration Tool
 * Copyright (C) 2026 TlFoxHuman (TranslucentFoxHuman,半狐,半透明狐人間)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winreg.h>

void print_help(void) {
    printf("CBRT - Custom web Browser Registration Tool for Windows\n"
           "\n"
           "Usage:\n"
           "cbrt.exe </i ID> </n NAME> </c COMMAND> [/d DESCRIPTION] [/h | -h | --help] [/v]\n"
           "\n"
           "Options:\n"
           "/n              : Specify the name of the application.\n"
           "/i              : Specify the ID of the application. You can specify an arbitrary string, but only half-width alphanumeric characters can be used, there should be no space and \\.\n"
           "/c              : Specify the command line to launch.\n"
           "/d              : Specify the description of the browser entry. (Optional)\n"
           "/v              : Print version information.\n"
           "/h /? -h --help : Print this help message.\n"
           "\n"
           "Usage examples:\n\n"
           "- Register \"C:\\Program Files\\MyBrowser\\Main.exe\" as a browser:\n"
           "cbrt.exe /i MyBrowser /n \"My custom browser\" /c \"\\\"C:\\Program Files\\MyBrowser\\Main.exe\\\" \\\"%%1\\\"\" /d \"My self-made browser application\"\n"
           "- Register Firefox that opens in private window:\n"
           "cbrt.exe /i FirefoxPrivate /n \"Firefox Private Window\" /c \"\\\"C:\\Program Files\\Mozilla Firefox\\firefox.exe\\\" --private-window \\\"%%1\\\"\"\n"
           );
}
void print_ver(void) {
    printf("CBRT - Custom Browser Registration Tool for Windows\n"
           "Version %s\n"
           "\n"
           "Copyright (C) 2026 TlFoxHuman (TranslucentFoxHuman,半狐,半透明狐人間)\n\n"
           "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n"
           "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details: <https://www.gnu.org/licenses/>\n"
           ,__PROG_VER__);
}


int main(int argc,char** argv) {
    
    char* browser_id=NULL;
    char* browser_name=NULL;
    char* browser_cmd=NULL;
    char* browser_desc=NULL;
    
    int i=0;

    HKEY regkey;
    LONG errCode=0;
    char* regkeypath = NULL;
    
    if (argc < 2) {
        print_help();
        exit(0);
    }
    for (i=1;i<argc;i++) {
        if (strcmp(argv[i],"/h") == 0|| strcmp(argv[i],"/?") == 0 || strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0) {
            print_help();
            exit(0);
        } else if (strcmp(argv[i],"/v") == 0 ) {
            print_ver();
            exit(0);
        } else if (strcmp(argv[i],"/i") == 0 ) {
            browser_id = (char*)malloc(strlen(argv[i+1])+1);
            strcpy(browser_id,argv[i+1]);
            i++;
        } else if (strcmp(argv[i],"/n") == 0 ) {
            browser_name = (char*)malloc(strlen(argv[i+1])+1);
            strcpy(browser_name,argv[i+1]);
            i++;
        } else if (strcmp(argv[i],"/c") == 0 ) {
            browser_cmd = (char*)malloc(strlen(argv[i+1])+1);
            strcpy(browser_cmd,argv[i+1]);
            i++;
        } else if (strcmp(argv[i],"/d") == 0 ) {
            browser_desc = (char*)malloc(strlen(argv[i+1])+1);
            strcpy(browser_desc,argv[i+1]);
            i++;
        } else {
            fprintf(stderr, "Unrecognized argument: %s\n",argv[i]);
            exit(1);
        }
    }
    
    printf("ID:%s\nName:%s\nCMD:%s\nDesc:%s\n",browser_id,browser_name,browser_cmd,browser_desc);

    // BEGIN HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\%s
    regkeypath = (char*)malloc(strlen("SOFTWARE\\Clients\\StartMenuInternet\\")+strlen(browser_id)+1);
    sprintf(regkeypath,"SOFTWARE\\Clients\\StartMenuInternet\\%s",browser_id);
    errCode = RegCreateKeyExA(HKEY_LOCAL_MACHINE,regkeypath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not open the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by: %d",regkeypath,(long)errCode);
        exit(2);
    }
    errCode = RegSetValueExA(regkey,NULL,0,REG_SZ,(const BYTE*)browser_id,strlen(browser_id)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by %d",regkeypath,(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    RegCloseKey(regkey);
    free(regkeypath);
    // END   HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\%s

    // BEGIN HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\%s\Capabilities
    regkeypath = (char*)malloc(strlen("SOFTWARE\\Clients\\StartMenuInternet\\\\Capabilities")+strlen(browser_id)+1);
    sprintf(regkeypath,"SOFTWARE\\Clients\\StartMenuInternet\\%s\\Capabilities",browser_id);
    errCode = RegCreateKeyExA(HKEY_LOCAL_MACHINE,regkeypath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not open the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by: %d",regkeypath,(long)errCode);
        exit(2);
    }
    errCode = RegSetValueExA(regkey,"ApplicationName",0,REG_SZ,(const BYTE*)browser_name,strlen(browser_name)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to ApplicationName in the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by %d",regkeypath,(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    if (browser_desc != NULL) {
        errCode = RegSetValueExA(regkey,"ApplicationDescription",0,REG_SZ,(const BYTE*)browser_desc,strlen(browser_desc)+1);
        if (errCode != ERROR_SUCCESS) {
            fprintf(stderr,"Error: Could not write value to ApplicationDescription in the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by %d",regkeypath,(long)errCode);
            RegCloseKey(regkey);
            exit(3);
        }
    }
    RegCloseKey(regkey);
    free(regkeypath);
    // END   HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\%s\Capabilities

    // BEGIN HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\%s\Capabilities\FileAssociations
    regkeypath = (char*)malloc(strlen("SOFTWARE\\Clients\\StartMenuInternet\\\\Capabilities\\FileAssociations")+strlen(browser_id)+1);
    sprintf(regkeypath,"SOFTWARE\\Clients\\StartMenuInternet\\%s\\Capabilities\\FileAssociations",browser_id);
    errCode = RegCreateKeyExA(HKEY_LOCAL_MACHINE,regkeypath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not open the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by: %d",regkeypath,(long)errCode);
        exit(2);
    }
    errCode = RegSetValueExA(regkey,".html",0,REG_SZ,(const BYTE*)browser_id,strlen(browser_id)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to .html in the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by %d",regkeypath,(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    errCode = RegSetValueExA(regkey,".htm",0,REG_SZ,(const BYTE*)browser_id,strlen(browser_id)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to .htm in the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by %d",regkeypath,(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    RegCloseKey(regkey);
    free(regkeypath);
    // END   HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\%s\Capabilities\FileAssociations

    // BEGIN HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\%s\Capabilities\URLAssociations
    regkeypath = (char*)malloc(strlen("SOFTWARE\\Clients\\StartMenuInternet\\\\Capabilities\\URLAssociations")+strlen(browser_id)+1);
    sprintf(regkeypath,"SOFTWARE\\Clients\\StartMenuInternet\\%s\\Capabilities\\URLAssociations",browser_id);
    errCode = RegCreateKeyExA(HKEY_LOCAL_MACHINE,regkeypath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not open the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by: %d",regkeypath,(long)errCode);
        exit(2);
    }
    errCode = RegSetValueExA(regkey,"http",0,REG_SZ,(const BYTE*)browser_id,strlen(browser_id)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to http in the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by %d",regkeypath,(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    errCode = RegSetValueExA(regkey,"https",0,REG_SZ,(const BYTE*)browser_id,strlen(browser_id)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to https in the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by %d",regkeypath,(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    RegCloseKey(regkey);
    free(regkeypath);
    // END   HKEY_LOCAL_MACHINE\SOFTWARE\Clients\StartMenuInternet\%s\Capabilities\URLAssociations

    // BEGIN HKEY_LOCAL_MACHINE\SOFTWARE\RegisteredApplications
    errCode = RegCreateKeyExA(HKEY_LOCAL_MACHINE,"SOFTWARE\\RegisteredApplications",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not open the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by: %d","SOFTWARE\\RegisteredApplications",(long)errCode);
        exit(2);
    }
    regkeypath = (char*)malloc(strlen("SOFTWARE\\Clients\\StartMenuInternet\\\\Capabilities")+strlen(browser_id)+1);
    sprintf(regkeypath,"SOFTWARE\\Clients\\StartMenuInternet\\%s\\Capabilities",browser_id);
    errCode = RegSetValueExA(regkey,browser_id,0,REG_SZ,(const BYTE*)regkeypath,strlen(regkeypath)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to %s in the registory key: HKEY_LOCAL_MACHINE\\%s\nCaused by %d",browser_id,"SOFTWARE\\RegisteredApplications",(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    RegCloseKey(regkey);
    free(regkeypath);
    // END   HKEY_LOCAL_MACHINE\SOFTWARE\RegisteredApplications

    // お疲れ様でした。ここからはHKEY_CLASSES_ROOTです。

    //うぎゃー

    // BEGIN HKEY_CLASSES_ROOT\%s
    errCode = RegCreateKeyExA(HKEY_CLASSES_ROOT,browser_id,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not open the registory key: %s\nCaused by: %d",browser_id,(long)errCode);
        exit(2);
    }
    regkeypath = (char*)malloc(strlen(" HTML Document")+strlen(browser_name)+1);
    sprintf(regkeypath,"%s HTML Document",browser_name);
    errCode = RegSetValueExA(regkey,NULL,0,REG_SZ,(const BYTE*)regkeypath,strlen(regkeypath)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to the registory key: HKEY_CLASSES_ROOT\\%s\nCaused by %d",browser_id,(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    RegCloseKey(regkey);
    free(regkeypath);
    // END   HKEY_CLASSES_ROOT\%s

    // BEGIN HKEY_CLASSES_ROOT\%s\Shell\Open\Command
        // loop for recursive creation
        regkeypath = (char*)malloc(strlen("\\Shell")+strlen(browser_id)+1);
        sprintf(regkeypath,"%s\\Shell",browser_id);
        errCode = RegCreateKeyExA(HKEY_CLASSES_ROOT,regkeypath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
        if (errCode != ERROR_SUCCESS) {
            fprintf(stderr,"Error: Could not open the registory key: HKEY_CLASSES_ROOT\\%s\nCaused by: %d",regkeypath,(long)errCode);
            exit(2);
        }
        RegCloseKey(regkey);
        free(regkeypath);
        regkeypath = (char*)malloc(strlen("\\Shell\\Open")+strlen(browser_id)+1);
        sprintf(regkeypath,"%s\\Shell\\Open",browser_id);
        errCode = RegCreateKeyExA(HKEY_CLASSES_ROOT,regkeypath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
        if (errCode != ERROR_SUCCESS) {
            fprintf(stderr,"Error: Could not open the registory key: HKEY_CLASSES_ROOT\\%s\nCaused by: %d",regkeypath,(long)errCode);
            exit(2);
        }
        RegCloseKey(regkey);
        free(regkeypath);
        // loop end
    regkeypath = (char*)malloc(strlen("\\Shell\\Open\\Command")+strlen(browser_id)+1);
    sprintf(regkeypath,"%s\\Shell\\Open\\Command",browser_id);
    errCode = RegCreateKeyExA(HKEY_CLASSES_ROOT,regkeypath,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS | KEY_WOW64_64KEY,NULL,&regkey,NULL);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not open the registory key: HKEY_CLASSES_ROOT\\%s\nCaused by: %d",regkeypath,(long)errCode);
        exit(2);
    }
    errCode = RegSetValueExA(regkey,NULL,0,REG_SZ,(const BYTE*)browser_cmd,strlen(browser_cmd)+1);
    if (errCode != ERROR_SUCCESS) {
        fprintf(stderr,"Error: Could not write value to the registory key: HKEY_CLASSES_ROOT\\%s\nCaused by %d",regkeypath,(long)errCode);
        RegCloseKey(regkey);
        exit(3);
    }
    RegCloseKey(regkey);
    free(regkeypath);
    // END   HKEY_CLASSES_ROOT\%s\Shell\Open\Command

    printf("%s was successfully registered as a web browser!\n",browser_name);

    return 0;
}