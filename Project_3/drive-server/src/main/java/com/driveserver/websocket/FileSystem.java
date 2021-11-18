package com.driveserver.websocket;

public class FileSystem {
    private Base personalFiles;
    private Base sharedFiles;



    public FileSystem(){
        this.personalFiles = new Folder("My files", "File system/");
    };

    public Base getFile(String route){
        return null;
    };

    public void createdFile(String name, String route, String text) {

    };

    public void createdFolder(String name, String route) {

    }

    public void deletedFile(String ) {

    }

}
