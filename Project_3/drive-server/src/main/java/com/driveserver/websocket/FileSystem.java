package com.driveserver.websocket;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import org.json.simple.parser.ParseException;
import org.json.simple.parser.JSONParser;

import java.util.ArrayList;

public class FileSystem {
    private JSONObject fileSystem;



    public FileSystem(){
        this.fileSystem = new JSONObject();
        this.fileSystem.put("My files", new JSONObject());
        this.fileSystem.put("Shared files", new JSONObject());
    };

    public FileSystem(String fileSystem) {
        JSONParser parser = new JSONParser();
        try {
            Object object = parser.parse(fileSystem);
            this.fileSystem = (JSONObject)object;
        } catch (ParseException e) {
            System.err.println("Error at: " + e.getPosition() + " message: " + e.getMessage());
        }
    };

    //
    public JSONObject getFile(String route){
        JSONObject file;
        Object obj;
        JSONArray children;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        for (int i = 1; i < foldersName.length; i++) {

            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if (i == foldersName.length - 1) {
                    if(file.get("type") == "file" && (file.get("name") + "." + file.get("extension")) == foldersName[i]) {
                        return file;
                    }
                }
                else {
                    if(file.get("type") == "folder" && file.get("name") == foldersName[i]) {
                        break;
                    }
                }
            }
        }
        return null;
    };

    // Liseth
    public JSONObject getFolder(String route){
        JSONObject file;
        Object obj;
        JSONArray children;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        for (int i = 1; i < foldersName.length; i++) {

            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if(file.get("type") == "folder" && file.get("name") == foldersName[i]) {
                    return file;
                }
            }
        }
        return null;
    };

    // Fabián
    // name.txt
    public void createdFile(String name, String route, String text) throws Exception{

    };

    // Fabián
    public void createdFolder(String name, String route) throws Exception{

    };

    // Pablo
    public void deleteFile(String route) {

    };

    // Liseth
    public void deleteFolder(String route) {

    };

    // Pablo
    public void copyFile(String route, String newRoute) {

    };

    // Liseth
    public void copyFolder(String route, String newRoute) {

    };

    // Pablo
    public void moveFile(String route, String newRoute) {

    };

    // Fabián
    public void moveFolder(String route, String newRoute) {

    };
}
