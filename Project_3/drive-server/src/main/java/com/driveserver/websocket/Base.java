package com.driveserver.websocket;

import java.time.format.DateTimeFormatter;
import java.time.LocalDateTime;

public abstract class Base {
    protected String name;
    protected Double size;
    protected String dateCreated;
    protected String route;


    public Base(String name, String route) {
        DateTimeFormatter dtf = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
        this.name =name;
        this.size = 1.0;
        this.dateCreated = dtf.format(LocalDateTime.now());
        this.route = route;
    }

    public Base(String name, Double size, String dateCreated, String route) {
        this.name = name;
        this.size = size;
        this.dateCreated = dateCreated;
        this.route = route;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Double getSize() {
        return size;
    }

    public void setSize(Double size) {
        this.size = size;
    }

    public String getDateCreated() {
        return dateCreated;
    }

    public void setDateCreated(String dateCreated) {
        this.dateCreated = dateCreated;
    }

    public String getRoute() {
        return route;
    }

    public void setRoute(String route) {
        this.route = route;
    }
}
