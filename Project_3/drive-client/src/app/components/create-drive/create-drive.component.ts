import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { Router } from '@angular/router';
import { User } from 'src/app/models/user';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-create-drive',
  templateUrl: './create-drive.component.html',
  styleUrls: ['./create-drive.component.css']
})
export class CreateDriveComponent implements OnInit {
  user:User;

  constructor(private router: Router, private webSocket : WebSocketService) { }

  ngOnInit(): void {
    this.user = new User();
  }


  createDrive(form: NgForm) {
    if(form.invalid) { return; }

    this.webSocket.createDriveSubscribe(this.user.username, this);

    WebSocketService.stompClient.send('/app/create-drive', {}, JSON.stringify({'username':this.user.username, 'size': this.user.size}));
  }

  validateUser(msg: any) {
    let body = JSON.parse(msg.body);
    if(body.status == 200) {
      this.router.navigateByUrl(`/drive-dashboard/${this.user.username}`);
      
    } else {
      console.log(body.data);
    }
    
  }

}
