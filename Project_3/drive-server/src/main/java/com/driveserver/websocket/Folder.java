package com.driveserver.websocket;

import java.util.ArrayList;

public class Folder extends  Base{
    private ArrayList<Base> children;

    public Folder(String name, String route) {
        super(name,route);
        this.children = new ArrayList<>();
    }
}
