import { Injectable } from '@angular/core';

import * as Stomp from 'stompjs';
import * as SockJS from 'sockjs-client';
import { DashboardComponent } from '../components/dashboard/dashboard.component';
import { CreateDriveComponent } from '../components/create-drive/create-drive.component';

@Injectable({
  providedIn: 'root'
})
export class WebSocketService {

  url: string = 'http://localhost:8080/server';
  static stompClient: any;

  constructor() {
  }

  connect() {
    console.log("Initialize WebSocket Connection");
    let ws = new SockJS(this.url);
    const that = this;
    WebSocketService.stompClient = Stomp.over(ws);

    return new Promise((resolve, reject) => {
      WebSocketService.stompClient.connect({}, function (frame: any) {
        resolve({status:200, data:frame});
      }, ()=> {reject({status:400, data:'Connection failed, try again'})}
      );
    });
  }


  disconnect() {
    return new Promise((resolve) => {
      if (WebSocketService.stompClient != null) {
        WebSocketService.stompClient.disconnect(() => {
        });
      }
      resolve({'status':200});
    })
  }

  loadFiles(username:string, dashboard: DashboardComponent) {
    return new Promise((resolve) => {
      WebSocketService.stompClient.subscribe(`/queue/files-${username}`, (msg:any)=> {
        dashboard.loadFiles(msg);
      });
      resolve({status:200,data:{}});
    })
  }


  createDriveSubscribe(username:string, component:CreateDriveComponent) {
    console.log(WebSocketService.stompClient)
    WebSocketService.stompClient.subscribe(`/queue/create-drive-${username}`, (msg:any) => {
      component.validateUser(msg);
    })
  }
}

