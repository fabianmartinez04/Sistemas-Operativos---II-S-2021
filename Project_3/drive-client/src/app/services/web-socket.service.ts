import { Injectable } from '@angular/core';

import * as Stomp from 'stompjs';
import * as SockJS from 'sockjs-client';

@Injectable({
  providedIn: 'root'
})
export class WebSocketService {

  url: string = 'http://localhost:8080/server';
  static stompClient: any;

  constructor() {
  }

  connect(username:string) {
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
      if (WebSocketService.stompClient !== null) {
        WebSocketService.stompClient.disconnect(() => {
          resolve({'status':200});
        });
      }
    })
  }
}

