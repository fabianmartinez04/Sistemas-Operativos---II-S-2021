package com.driveserver.websocket;

public class File extends Base{
    private String fileExtension;
    private String modifiedDate;
    private String text;


    public File(String name, String route){
        super(name, route);
    }
}
