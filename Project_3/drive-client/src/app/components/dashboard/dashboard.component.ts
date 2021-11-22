import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { User } from 'src/app/models/user';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrls: ['./dashboard.component.css']
})
export class DashboardComponent implements OnInit {


  user:User;
  files: File[] = [];
  personalFiles:boolean = true;
  selectedItem : number = -1;
  path:string= 'MyFiles';
  fileSystem: JSON;

  constructor(private router: Router, private activatedRouter : ActivatedRoute, private websocket: WebSocketService) {
  }

  ngOnInit(): void {
    this.user = new User();
    this.activatedRouter.params.subscribe((params:any)=> {
      this.user.username = params['username'];

      // call a service to get files of directory
      this.websocket.loadFiles(this.user.username, this)
        .then((data:any) => {
          WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({username:this.user.username, path:this.path}))
        });

    })
  }


  exit() {
    this.websocket.disconnect()
      .then((data:any)=>{
        this.router.navigateByUrl('/user-login');
      })
      .catch((data) => {
        
      })
  }

  loadFiles(msg: any) {
   let data = JSON.parse(msg.body);
      if (data.code == 200) {
        this.fileSystem = data.data;
        console.log(this.fileSystem )
      } else {
        console.log(data.data);
      }
  }

  loadPersonalFiles() {
    this.personalFiles = true;
    this.path = 'MyFiles';
    WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({username:this.user.username, path:this.path}))
  }

  loadSharedFiles() {
    this.personalFiles = false;
    this.path = 'SharedFiles';
    WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({username:this.user.username, path:this.path}))
  }

  openFile(fileIndex: number) {
    if(this.files[fileIndex].type == "folder") {
      this.path = this.path + '/'+ this.files[fileIndex].fileName;
    }
    console.log("this is dobule click");
  }

  goBack() {
    let files : string[] = this.path.split('/');
    if(files.length == 1){return;}
    files.pop();
    this.path = files.join('/');
  }

}
