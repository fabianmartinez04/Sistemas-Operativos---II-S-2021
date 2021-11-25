package com.driveserver.websocket;

import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import org.json.simple.parser.ParseException;
import org.json.simple.parser.JSONParser;


import javax.swing.*;
import java.io.*;
import java.text.SimpleDateFormat;

import java.util.ArrayList;
import java.util.Date;

public class FileSystem {
    private JSONObject fileSystem;

    public JSONObject getFileSystem() {
        return fileSystem;
    }

    public void setFileSystem(JSONObject fileSystem) {
        this.fileSystem = fileSystem;
        System.out.println(this.fileSystem);
    }

    public FileSystem(){};

    public JSONObject getFileSystem(String username, Integer size, Boolean isNew) throws  Exception{
        JSONObject file;
        JSONObject auxFile;
        if (isNew) {
            this.fileSystem = new JSONObject();
            this.fileSystem.put("My files", new JSONObject());
            this.fileSystem.put("Shared files", new JSONObject());
            // create a filesystem
            file = new JSONObject();
            file.put("username", username);
            file.put("size", size);

            JSONObject aux = new JSONObject();
            aux.put("children", new JSONArray());
            file.put("MyFiles", aux);

            aux = new JSONObject();
            aux.put("children", new JSONArray());
            file.put("SharedFiles", aux);


            File f = new File(  "FileSystems",username + ".json");
            if (f.exists()) {
                throw new Exception("File already exist, please try again");
            } else {
                FileWriter writer = new FileWriter("src/FileSystems/" + username + ".json");
                writer.write(file.toJSONString());
                writer.close();
            }
        } else {
            try {
                JSONParser parser = new JSONParser();
                FileReader reader = new FileReader("src/FileSystems/" + username + ".json");
                Object obj = parser.parse(reader);
                file = (JSONObject) obj;
                reader.close();
            } catch (ParseException e) {
                System.err.println("Error at: " + e.getPosition() + " message: " + e.getMessage());
                throw new Exception("File system no found");
            }

        }
        this.fileSystem = file;
        return  file;
    };

    private void updateJson() throws Exception {
        FileWriter writer = new FileWriter("src/FileSystems/" + this.fileSystem.get("username") + ".json");
        writer.write(this.fileSystem.toJSONString());
        writer.close();
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
                    if((file.get("type").equals("file")) && ((file.get("name") + "." + file.get("extension")).equals(foldersName[i]))) {

                        return file;
                    }
                }
                else {
                    if((file.get("type").equals("folder")) && (file.get("name").equals(foldersName[i]))) {
                        break;
                    }
                }
            }
        }
        return null;
    };

    // Liseth
    public JSONObject getFolder(String route){
        JSONObject folder;
        Object obj;
        JSONArray children;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        folder = (JSONObject) obj;

        for (int i = 1; i < foldersName.length; i++) {
            obj = folder.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++) {
                obj = children.get(j);
                folder = (JSONObject) obj;

                if (folder.get("type").equals("folder") && folder.get("name").equals(foldersName[i])) {
                    if (i == foldersName.length - 1) {
                        return folder;
                    } else {
                        break;
                    }
                }

            }

        }

        return folder;
    };

    // Fabián
    // name.txt
    public void createdFile(String name, String extension, String route, String text) throws Exception{
        JSONObject file;
        Object obj;
        JSONArray children;
        int size;
        Date date = new Date();
        SimpleDateFormat formatter = new SimpleDateFormat("dd-MM-yyyy HH:mm:ss");

        String[] foldersName = route.split("/");

        size = text.length();
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        if (foldersName.length == 1) {

            JSONObject newFile = new JSONObject();
            newFile.put("type","file");
            newFile.put("extension", extension);
            newFile.put("name", name);
            newFile.put("dateCreated",(formatter.format(date)).toString());
            newFile.put("modifiedCreated",(formatter.format(date)).toString());
            newFile.put("size",String.valueOf(size));
            newFile.put("route",route);
            newFile.put("text",text);

            Object obj2  = file.get("children");
            JSONArray list = (JSONArray)obj2;
            list.add(newFile);
            file.replace("children",list);
            updateJson();
            return;
        }

        for (int i = 1; i < foldersName.length; i++) {

            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if((file.get("type").equals("folder")) && (file.get("name").equals(foldersName[i]))) {

                    if (i == foldersName.length - 1){
                        JSONObject newFile = new JSONObject();
                        newFile.put("type","file");
                        newFile.put("extension",extension);
                        newFile.put("name",name);
                        newFile.put("dateCreated",(formatter.format(date)).toString());
                        newFile.put("modifiedCreated",(formatter.format(date)).toString());
                        newFile.put("size",String.valueOf(size));
                        newFile.put("route",route);
                        newFile.put("text",text);

                        Object obj2  = file.get("children");
                        JSONArray list = (JSONArray)obj2;
                        list.add(newFile);
                        file.replace("children",list);
                        updateJson();

                    }else{
                        break;
                    }


                }
            }
        }

    };

    // Fabián
    public void createdFolder(String name, String route) throws Exception{
        JSONObject file;
        Object obj;
        JSONArray children;
        Date date = new Date();
        System.out.println(" sdadad" + route);
        SimpleDateFormat formatter = new SimpleDateFormat("dd-MM-yyyy HH:mm:ss");

        String[] foldersName = route.split("/");

        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        if (foldersName.length == 1) {
            JSONObject newFolder = new JSONObject();
            newFolder.put("type","folder");
            newFolder.put("name",name);
            newFolder.put("dateCreated",(formatter.format(date)).toString());
            JSONArray array = new JSONArray();
            newFolder.put("children",array);
            newFolder.put("route",route);

            Object obj2  = file.get("children");
            JSONArray list = (JSONArray)obj2;
            list.add(newFolder);
            file.replace("children",list);
            updateJson();
        }

        for (int i = 1; i < foldersName.length; i++) {

            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if((file.get("type").equals("folder")) && (file.get("name").equals(foldersName[i]))) {
                    if(i == foldersName.length - 1) {
                        JSONObject newFolder = new JSONObject();
                        newFolder.put("type","folder");
                        newFolder.put("name",name);
                        newFolder.put("dateCreated",(formatter.format(date)).toString());
                        JSONArray array = new JSONArray();
                        newFolder.put("children",array);
                        newFolder.put("route",route);

                        Object obj2  = file.get("children");
                        JSONArray list = (JSONArray)obj2;
                        list.add(newFolder);
                        file.replace("children",list);
                        updateJson();
                    } else {
                        break;
                    }
                }
            }
        }

    };
    public JSONObject editFile(String route, String text) throws Exception{
        JSONObject file;
        Object obj;
        JSONArray children;

        Date date = new Date();
        SimpleDateFormat formatter = new SimpleDateFormat("dd-MM-yyyy HH:mm:ss");

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        if (foldersName.length == 2) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if((file.get("type").equals("file")) && ((file.get("name") + "." + file.get("extension")).equals(foldersName[1]))) {
                    file.replace("text", text);
                    updateJson();
                    return file;
                }
            }
        }

        for (int i = 1; i < foldersName.length; i++) {

            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if (i == foldersName.length - 1) {
                    if((file.get("type").equals("file")) && ((file.get("name") + "." + file.get("extension")).equals(foldersName[i]))) {
                        file.replace("text", text);
                        file.replace("modifiedCreated", (formatter.format(date)).toString());
                        updateJson();
                        return file;
                    }
                }
                else {
                    if((file.get("type").equals("folder")) && (file.get("name").equals(foldersName[i]))) {
                        break;
                    }
                }
            }
        }
        return null;

    };

    // Pablo
    public JSONObject deleteFile(String route) throws Exception {

        JSONObject file;
        Object obj;
        JSONArray children;
        JSONObject parent;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;
        if (foldersName.length == 2) {

            obj = file.get("children");
            children = (JSONArray) obj;

            // save the parent of the current children
            parent = file;
            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;

                if ((file.get("type").equals("file")) && ((file.get("name") + "." + file.get("extension")).equals(foldersName[1]))) {

                    // remove the file target from children
                    children.remove(file);
                    // add children again
                    parent.replace("children", children);
                    updateJson();
                    return file;
                }
            }
        }

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
                    System.out.println("FILE \n");
                    System.out.println(file.toJSONString() + '\n');
                    if ((file.get("type").equals("file")) && ((file.get("name") + "." + file.get("extension")).equals(foldersName[i]))) {
                        // remove the file target from children
                        children.remove(file);
                        // add children again
                        parent.replace("children", children);
                        updateJson();
                        return file;
                    }
                }
                if((file.get("type").equals("folder")) && (file.get("name").equals(foldersName[i]))) {
                    break;
                }
            }
        }
        return  null;
    };

    // Liseth
    public JSONObject deleteFolder(String route) throws Exception {
        JSONObject folder;
        Object obj;
        JSONArray children;
        JSONObject parent;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        folder = (JSONObject) obj;

        for (int i = 1; i < foldersName.length; i++) {

            obj = folder.get("children");
            children = (JSONArray) obj;
            // save the parent of the current children
            parent = folder;
            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                folder = (JSONObject)obj;
                // add folder to new children list
                // if this folder is parent folder of the folder to remove

                if((folder.get("type").equals("folder")) && (folder.get("name").equals(foldersName[i]))) {
                    if ((i == foldersName.length - 1)){

                        // remove the file target from children

                        children.remove(folder);
                        // add children again
                        if (children.size() == 0){
                            parent.replace("children", new JSONArray());
                        }else{
                            parent.replace("children", children);
                        }

                        updateJson();
                        return folder;

                    }else{
                        break;
                    }

                }

            }
        }
        return  null;

    };

    // Pablo
    public void copyFile(String route, String newRoute) throws Exception {
        JSONObject file;
        Object obj;
        JSONArray children;
        JSONObject target = null;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        // looking for file target
        if (foldersName.length == 1) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if((file.get("type").equals("file")) && ((file.get("name") + "." + file.get("extension")).equals(foldersName[1]))) {
                    target = (JSONObject) file.clone();
                    break;
                }
            }
        }

        for (int i = 1; i < foldersName.length && target == null; i++) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if (i == foldersName.length - 1) {
                    if((file.get("type").equals("file")) && ((file.get("name") + "." + file.get("extension")).equals(foldersName[i]))) {
                        target = (JSONObject) file.clone();
                        break;
                    }
                }
                else {
                    if((file.get("type").equals("folder")) && (file.get("name").equals(foldersName[i]))) {
                        break;
                    }
                }
            }
        }

        // looking path to copy file target
        foldersName = newRoute.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        if (foldersName.length == 1) {

            obj = file.get("children");
            children = (JSONArray) obj;

            Object obj2  = file.get("children");
            JSONArray list = (JSONArray)obj2;
            target.replace("route",newRoute);
            list.add(target);
            file.replace("children",list);
            updateJson();
        }

        for (int i = 1; i < foldersName.length; i++) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if((file.get("type").equals("folder")) && ((file.get("name")).equals(foldersName[i]))) {
                    if (i == foldersName.length-1) {
                        Object obj2  = file.get("children");
                        JSONArray list = (JSONArray)obj2;
                        target.replace("route",newRoute);
                        list.add(target);
                        file.replace("children",list);
                        updateJson();
                        break;
                    } else {
                        break;
                    }
                }
            }
        }
    };


    // Liseth
    public void copyFolder(String route, String newRoute) throws Exception {
        JSONObject folder;
        Object obj;
        JSONArray children;
        JSONObject target = null;

        String[] foldersName = route.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        folder = (JSONObject) obj;

        // looking for folder
        if (foldersName.length == 2) {
            obj = folder.get("children");
            children = (JSONArray) obj;
            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                folder = (JSONObject)obj;
                if((folder.get("type").equals("folder")) && (folder.get("name").equals(foldersName[1]))) {
                    target = (JSONObject) folder.clone();
                    break;
                }
            }
        }

        for (int i = 1; i < foldersName.length && target == null; i++) {
            obj = folder.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                folder = (JSONObject)obj;

                if((folder.get("type").equals("folder")) && (folder.get("name").equals(foldersName[i]))) {
                    if (i == foldersName.length - 1) {
                        target = (JSONObject) folder.clone();
                        break;
                    } else {
                        break;
                    }
                }
            }
        }

        // copy target folder in destination folder
        foldersName = newRoute.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        folder = (JSONObject) obj;

        if (foldersName.length == 1) {
            Object obj2  = folder.get("children");
            JSONArray list = (JSONArray)obj2;
            target.replace("route",newRoute);
            list.add(target);
            folder.replace("children",list);
            updateJson();
        }

        for (int i = 1; i < foldersName.length; i++) {
            obj = folder.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                folder = (JSONObject)obj;
                if((folder.get("type").equals("folder")) && (folder.get("name").equals(foldersName[i]))) {
                    if (i == foldersName.length-1) {
                        Object obj2  = folder.get("children");
                        JSONArray list = (JSONArray)obj2;
                        target.replace("route",newRoute);
                        list.add(target);
                        folder.replace("children",list);
                        updateJson();
                        break;
                    } else {
                        break;
                    }
                }
            }
        }
    };


    // Pablo
    public void moveFile(String route, String newRoute) throws Exception {

        JSONObject file;
        Object obj;
        JSONArray children;
        JSONObject target = null;
        String[] foldersName;
        // remove file from old path and return the file
        System.out.println("RUTA \n");
        System.out.println(route + '\n');
        target = deleteFile(route);

        // save the file in the new path
        foldersName = newRoute.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        if (foldersName.length == 1) {
            Object obj2  = file.get("children");
            JSONArray list = (JSONArray)obj2;
            target.replace("route", newRoute);
            list.add(target);
            file.replace("children",list);
            updateJson();

        }

        for (int i = 1; i < foldersName.length; i++) {

            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){

                obj = children.get(j);
                file = (JSONObject)obj;
                if((file.get("type").equals("folder")) && (file.get("name").equals(foldersName[i]))) {
                    if (i == foldersName.length-1) {
                        Object obj2  = file.get("children");
                        JSONArray list = (JSONArray)obj2;
                        target.replace("route", newRoute);
                        list.add(target);
                        file.replace("children",list);
                        updateJson();
                    } else {
                        break;
                    }
                }
            }
        }
    };

    // Fabián
    public void moveFolder(String route, String newRoute) throws Exception {
        JSONObject file;
        Object obj;
        JSONArray children;
        JSONObject target = null;
        JSONObject parent;
        String[] foldersName;
        // remove file from old path and return the file
        target = deleteFolder(route);
        System.out.println("TARGET");
        System.out.println(target);
        // save the file in the new path
        foldersName = newRoute.split("/");
        obj = (Object)this.fileSystem.get(foldersName[0]);
        file = (JSONObject) obj;

        if(foldersName.length == 1){
            Object obj2  = file.get("children");
            JSONArray list = (JSONArray)obj2;
            target.replace("route", newRoute);
            list.add(target);
            System.out.println("LISTA");
            System.out.println(list);
            file.replace("children",list);
            updateJson();
            return;

        }

        for (int i = 1; i < foldersName.length; i++) {
            obj = file.get("children");
            children = (JSONArray) obj;

            for (int j = 0; j < children.size(); j++){
                obj = children.get(j);
                file = (JSONObject)obj;
                if((file.get("type").equals("folder")) && (file.get("name").equals(foldersName[i]))) {
                    if (i == foldersName.length-1) {
                        Object obj2  = file.get("children");
                        JSONArray list = (JSONArray)obj2;
                        target.replace("route", newRoute);
                        list.add(target);
                        System.out.println("LISTA");
                        System.out.println(list);
                        file.replace("children",list);
                        updateJson();
                    } else {
                        break;
                    }
                }
            }
        }

    };
    //Liseth
    public JSONObject getSharedFiles() {
        JSONObject actualFileSystem = this.fileSystem;
        //username file system
        JSONObject targetFileSystem;
        JSONObject sharedFiles;
        JSONArray children;

        //All shared files uploaded
        JSONArray newChildren = new JSONArray();

        //Each shared file
        JSONObject sharedFile;
        String[] path;

        //Route to get folder or file
        String route;

        try {
            sharedFiles = (JSONObject) actualFileSystem.get("SharedFiles");
            children = (JSONArray) sharedFiles.get("children");

            for (int i = 0; i < children.size(); i++){
                //Get each shared file
                sharedFile = (JSONObject) children.get(i);

                path = sharedFile.get("path").toString().split("/");

                targetFileSystem = getFileSystem(path[0],0,false);

                this.setFileSystem(targetFileSystem);

                //Delete username/myfiles/notas.txt from path
                route = sharedFile.get("path").toString().substring(sharedFile.get("path").toString().indexOf("/")+1);

                route.trim();

                if(path[path.length-1].contains(".")){
                    sharedFile = this.getFile(route);


                }
                else{
                    sharedFile = this.getFolder(route);
                }
                newChildren.add(sharedFile);
            }
            sharedFiles.replace("children",newChildren);
            actualFileSystem.replace("SharedFiles",sharedFiles);

            this.setFileSystem(actualFileSystem);

        }
        catch (Exception e){
            System.out.println("getSharedFiles ERROR");

        }
    //    System.out.println(actualFileSystem.toJSONString());
        return actualFileSystem;
    }
    public void shareFile(String username,String usertoshare,String path){
        JSONObject actualFileSystem = this.fileSystem;
        JSONObject sharedFiles;
        JSONArray children;
        JSONObject newSharedFile= new JSONObject();
        JSONObject usernameFileSystem;
        try{

            JSONParser parser = new JSONParser();
            FileReader reader = new FileReader("src/FileSystems/" + usertoshare + ".json");
            Object obj = parser.parse(reader);
            usernameFileSystem = (JSONObject) obj;

            sharedFiles = (JSONObject) usernameFileSystem.get("SharedFiles");
            children = (JSONArray) sharedFiles.get("children");
            newSharedFile.put("path",username + "/" + path);
            children.add(newSharedFile);
            usernameFileSystem.replace("children",children);
            this.setFileSystem(usernameFileSystem);
            this.updateJson();
            this.setFileSystem(actualFileSystem);

            System.out.println("shareFile SUCCESS");

        }
        catch (Exception e){
            System.out.println("shareFile ERROR");
        }

    }

}
