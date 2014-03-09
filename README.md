modular-server
==============

Title: Modular server

- Author   :  Pavol Jancura (pavol.jancura@gmail.com)
- Platform :  Windows 
- Langauge :  C++
- Version  :  1.0



Descritpion: A programmable server which functionality can easily be extended by adding plugins (DDLs).

    This project contains the working implementation of a server, which functionality can be easily extended by
    plugins implementated as dll. For the purpose of demonstration, few plugins are implemeneted in DLL folder 
    enabling some basic functionalies of a server such as: echo, broadcast, chat, ping,... . 
    One can connect to the server using TELNET. The server without any extended functionality implements only 
    basic commands: who; whoami; close; exit. The server is configured by 'server.cfg' file in a root directory.
    The current documnentation is in Slovak language.

The main repository content:

    _exe_          : The instalation folder containg executables. This is the current working final vesrion. 
                     It does not contain a source code.

    DLL            : The folder contains the source code of plugins and libraies for the server implemneted 
                     as dynamnicly linked libraries (DLLs). They can be imported as MS Visual Studio projects.

    Modular Server : The folder containing the source code of the server. It can be imported as MS Visual Studio project.

    Programova dokumentacia.doc   : The program documentaion in Slovak language.
    Uzivatelska dokumentacia.doc  : The user documentaion in Slovak language.
    LICENSE                       : The license terms.
