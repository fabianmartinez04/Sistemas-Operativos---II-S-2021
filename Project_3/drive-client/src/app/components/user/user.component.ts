import { stringify } from '@angular/compiler/src/util';
import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { Router } from '@angular/router';
import { User } from 'src/app/models/user';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-user',
  templateUrl: './user.component.html',
  styleUrls: ['./user.component.css']
})
export class UserComponent implements OnInit {

  user: User;

  constructor(private router: Router, private webSocket: WebSocketService) { }

  ngOnInit(): void {
    this.user = new User();
  }


  login(form: NgForm) {
    if (form.invalid) { return; }

    this.webSocket.connect()
      .then((data: any) => {
          
          WebSocketService.stompClient.subscribe('/queue/login-' + this.user.username, (msg:any) => {
            let obj = JSON.parse(msg.body)
            if (obj.status == 200) {
              this.router.navigateByUrl(`/drive-dashboard/${this.user.username}`);
              WebSocketService.stompClient.unsubscribe('/queue/login-' + this.user.username);
            } else {
              console.log('ERROR');
            }
          })
      })
      .finally(() => {
        WebSocketService.stompClient.send('/app/login', {}, JSON.stringify({username:this.user.username}))
      })
      .catch((msg: any) => {
        console.log(msg);
      })
  }

}
