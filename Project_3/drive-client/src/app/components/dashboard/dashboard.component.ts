import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { User } from 'src/app/models/user';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';
import { Handler } from 'src/app/models/handler';

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrls: ['./dashboard.component.css']
})
export class DashboardComponent implements OnInit {


  user: User;
  files: File[] = [];
  personalFiles: boolean = true;
  selectedItem: number = -1;
  path: string = 'MyFiles';
  fileSystem: JSON;
  private handler: Handler = new Handler;

  constructor(private router: Router, private activatedRouter: ActivatedRoute, private websocket: WebSocketService) {
  }

  ngOnInit(): void {
    this.user = new User();
    this.activatedRouter.params.subscribe((params: any) => {
      this.user.username = params['username'];

      // call a service to get files of directory
      this.websocket.loadFiles(this.user.username, this)
        .then((data: any) => {
          WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({ username: this.user.username, path: this.path }))
        });

    })
  }


  exit() {
    this.router.navigateByUrl('/user-login');
  }

  loadFiles(msg: any) {
    let data = JSON.parse(msg.body);
    if (data.status == 200) {
      this.fileSystem = data.data;
      this.files = this.handler.loadFileOfPath(this.fileSystem);
    } else {
      console.log(data.data);
    }
  }

  loadPersonalFiles() {
    this.selectedItem = -1;
    this.personalFiles = true;
    this.path = 'MyFiles';
    WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({ username: this.user.username, path: this.path }))
  }

  loadSharedFiles() {
    this.selectedItem = -1;
    this.personalFiles = false;
    this.path = 'SharedFiles';
    WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({ username: this.user.username, path: this.path }))
  }

  goBack() {
    let files: string[] = this.path.split('/');
    if (files.length == 1) { return; }
    files.pop();
    this.path = files.join('/');
  }

}
