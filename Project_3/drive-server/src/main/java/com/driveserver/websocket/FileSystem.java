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
    public JSONObject deleteFile(String route) {
        JSONObject file;
        Object obj;
        JSONArray children;
        JSONObject parent;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        for (int i = 1; i < foldersName.length; i++) {

            obj = file.get("children");
            children = (JSONArray) obj;
            // save the parent of the current children
            parent = file;
            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                // add file to new children list
                // if this folder is parent folder of the file to remove
                if (i == foldersName.length - 1) {
                    if(file.get("type") == "file" && (file.get("name") + "." + file.get("extension")) == foldersName[i]){
                        // remove the file target from children
                        children.remove(file);
                        // add children again
                        parent.replace("children", children);
                        return file;
                    }
                } else {
                    if(file.get("type") == "folder" && file.get("name") == foldersName[i]) {
                        break;
                    }
                }
            }
        }
        return  null;
    };

    // Liseth
    public void deleteFolder(String route) {

    };

    // Pablo
    public void copyFile(String route, String newRoute) {
        JSONObject file;
        Object obj;
        JSONArray children;
        JSONObject target = null;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        for (int i = 1; i < foldersName.length && target == null; i++) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if (i == foldersName.length - 1) {
                    if(file.get("type") == "file" && (file.get("name") + "." + file.get("extension")) == foldersName[i]) {
                        target = file;
                        break;
                    }
                }
                else {
                    if(file.get("type") == "folder" && file.get("name") == foldersName[i]) {
                        break;
                    }
                }
            }
        }

        foldersName = newRoute.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        for (int i = 1; i < foldersName.length; i++) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if(file.get("type") == "folder" && file.get("name") == foldersName[i]) {
                    if (i == foldersName.length-1) {
                        Object obj2  = file.get("children");
                        JSONArray list = (JSONArray)obj2;
                        target.replace("route",newRoute);
                        list.add(target);
                        file.replace("children",list);
                    } else {
                        break;
                    }
                }
            }
        }
    };

    // Liseth
    public void copyFolder(String route, String newRoute) {

    };

    // Pablo
    public void moveFile(String route, String newRoute) {
        JSONObject file;
        Object obj;
        JSONArray children;
        JSONObject target = null;
        JSONObject parent;
        String[] foldersName;
        // remove file from old path and return the file
        target = deleteFile(route);

        // save the file in the new path
        foldersName = newRoute.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        for (int i = 1; i < foldersName.length; i++) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if(file.get("type") == "folder" && file.get("name") == foldersName[i]) {
                    if (i == foldersName.length-1) {
                        Object obj2  = file.get("children");
                        JSONArray list = (JSONArray)obj2;
                        target.replace("route", newRoute);
                        list.add(target);
                        file.replace("children",list);
                    } else {
                        break;
                    }
                }
            }
        }
    };

    // Fabián
    public void moveFolder(String route, String newRoute) {
        JSONObject file;
        Object obj;
        JSONArray children;
        JSONObject target = null;
        JSONObject parent;
        String[] foldersName;
        // remove file from old path and return the file
        target = deleteFolder(route);

        // save the file in the new path
        foldersName = newRoute.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        for (int i = 1; i < foldersName.length; i++) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if(file.get("type") == "folder" && file.get("name") == foldersName[i]) {
                    if (i == foldersName.length-1) {
                        Object obj2  = file.get("children");
                        JSONArray list = (JSONArray)obj2;
                        target.replace("route", newRoute);
                        list.add(target);
                        file.replace("children",list);
                    } else {
                        break;
                    }
                }
            }
        }

    };
}
