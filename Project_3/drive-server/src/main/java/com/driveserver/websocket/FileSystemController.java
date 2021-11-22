package com.driveserver.websocket;

import org.json.simple.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.Message;
import org.springframework.messaging.handler.annotation.Header;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.Payload;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.messaging.simp.annotation.SendToUser;
import org.springframework.stereotype.Controller;
import org.springframework.web.util.HtmlUtils;

import java.security.Principal;
import java.text.SimpleDateFormat;
import java.util.Date;

@Controller
public class FileSystemController {
    @Autowired
    SimpMessagingTemplate sender;
    private FileSystem fileSystem = new FileSystem();

    @MessageMapping("/login")
   // @SendTo()
    public void validateUserFileSystem(@Payload JSONObject obj) {
        // look for username file system existing
        try {
            fileSystem.getFileSystem(obj.get("username").toString(), 0,false);
            JSONObject out = new JSONObject();
            out.put("status", 200);
            out.put("data", new JSONObject());
            sender.convertAndSend("/queue/login-" + obj.get("username"), out.toJSONString());
        }catch (Exception e) {
            JSONObject out = new JSONObject();
            out.put("status", 400);
            out.put("data", "User no found");
            sender.convertAndSend("/queue/login-" + obj.get("username"), out.toJSONString());
        }

        JSONObject out = new JSONObject();
        out.put("status", 200);
        out.put("data", new JSONObject());

    }

    @MessageMapping("/loadFiles")
    public void sendFilesToClient(@Payload JSONObject obj) {
        try {
            JSONObject userFileSystem = fileSystem.getFileSystem(obj.get("username").toString(), 0,Boolean.FALSE);
            fileSystem.setFileSystem(userFileSystem);
            userFileSystem = fileSystem.getFolder(obj.get("path").toString());
            JSONObject out = new JSONObject();
            out.put("status", 200);
            out.put("data",userFileSystem);
            sender.convertAndSend("/queue/files-" + obj.get("username"), out.toJSONString());
        }catch (Exception e) {
            JSONObject out = new JSONObject();
            out.put("status", 400);
            out.put("data", e.getMessage());
            sender.convertAndSend("/queue/files-" + obj.get("username"), out.toJSONString());
        }
    }


    @MessageMapping("/create-drive")
    public void createNewDrive(@Payload JSONObject obj) {
        try {
            JSONObject userFileSystem = fileSystem.getFileSystem(obj.get("username").toString(), Integer.parseInt(obj.get("size").toString()), Boolean.TRUE);
            JSONObject out = new JSONObject();
            out.put("status", 200);
            out.put("data",userFileSystem);
            sender.convertAndSend("/queue/create-drive-" + obj.get("username"), out.toJSONString());
        }catch (Exception e) {
            JSONObject out = new JSONObject();
            out.put("status", 400);
            out.put("data", e.getMessage());
            sender.convertAndSend("/queue/create-drive-" + obj.get("username"), out.toJSONString());
        }
    }








    @MessageMapping("/news")
    @SendTo("/topic/news")
    public String broadcastNews(@Payload String message) {
        return message;
    }

    @MessageMapping("/hello")
    @SendTo("/topic/news")
    public Greeting greet(HelloMessage message) throws InterruptedException {
        return new Greeting("Hello, " +
                HtmlUtils.htmlEscape(message.getName()));
    }



}
