import { Component } from '@angular/core';
import { WebSocketService } from './services/web-socket.service';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'drive-client';

  greetings: string[] = [];
  name: string = '';


  constructor(private webSocket : WebSocketService) {
    this.webSocket.connect();
  }


  


}
