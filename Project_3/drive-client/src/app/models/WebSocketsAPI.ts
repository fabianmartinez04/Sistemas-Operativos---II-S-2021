import { AppComponent } from '../app.component';

import * as Stomp from 'stompjs';
import * as SockJS from 'sockjs-client';


export class WebSocketAPI {
    url: string = 'http://localhost:8080/server';
    topic: string = '/topic/greetings';
    stompClient: any;

    appComponent: AppComponent;
    constructor(appComponent: AppComponent) {
        this.appComponent = appComponent;
    }


    _connect() {
        console.log("Initialize WebSocket Connection");
        let ws = new SockJS(this.url);
        this.stompClient = Stomp.over(ws);
        const _this = this;
        _this.stompClient.connect({}, function (frame: any) {
            _this.stompClient.subscribe(_this.topic, function (sdkEvent: any) {
                _this.onMessageReceived(sdkEvent);
            });
        }, this.errorCallBack);
    };

    _disconnect() {
        if (this.stompClient !== null) {
            this.stompClient.disconnect();
        }
        console.log("Disconnected");
    }

    // on error, schedule a reconnection attempt
    errorCallBack(error:string) {
        console.log("errorCallBack -> " + error)
        setTimeout(() => {
            this._connect();
        }, 5000);
    }

     /**
  * Send message to sever via web socket
  * @param {*} message 
  */
    _send(message:string) {
        console.log("calling logout api via web socket");
        console.log(JSON.stringify(message));
        this.stompClient.send("/app/hello", {}, JSON.stringify(message));
    }

    onMessageReceived(message:any) {
        console.log("Message Recieved from Server :: " + message);
        this.appComponent.handleMessage(JSON.parse(message.body).message);
    }
}
