package com.driveserver.websocket;

import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.stereotype.Controller;
import org.springframework.web.util.HtmlUtils;

@Controller
public class GreetingController {

    @MessageMapping("/hello")
    @SendTo("/topic/greetings")
    public Greeting greet(HelloMessage message) throws InterruptedException {
        return new Greeting("Hello, " +
                HtmlUtils.htmlEscape(message.getName()));
    }


    @MessageMapping("/new-message")
    @SendTo("/Topic/greetings")
    public Greeting newMessage(String message) throws  InterruptedException {
            return new Greeting(message);
    }


}
