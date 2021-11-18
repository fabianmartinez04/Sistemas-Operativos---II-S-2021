import { Component } from '@angular/core';
import { WebSocketAPI } from './models/WebSocketsAPI';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'drive-client';

  webSocketAPI: WebSocketAPI =  new WebSocketAPI(this);;
  greetings: string[] = [];
  name: string = '';


  connect(){
    this.webSocketAPI._connect();
  }

  disconnect(){
    this.webSocketAPI._disconnect();
  }

  sendMessage(){
    this.webSocketAPI._send(this.name);
  }

  handleMessage(message:any){
    console.log(this.greetings)
    this.greetings.push(message);
  }
}
