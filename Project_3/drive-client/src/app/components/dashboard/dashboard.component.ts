import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { User } from 'src/app/models/user';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';
import { Handler } from 'src/app/models/handler';

import $ from 'jquery';
import { UtilService } from 'src/app/services/util.service';

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
  fileSystem: any;
  pathSelected: string = 'MyFiles';
  modalShow:boolean = false;
  fileToOpen: File = new File();

  private handler: Handler = new Handler;

  constructor(private router: Router, private activatedRouter: ActivatedRoute, private websocket: WebSocketService, private utilService : UtilService) {
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
      
      if (this.fileSystem.name != null) {
        this.path = this.fileSystem.route + '/' + this.fileSystem.name;
      }
      this.pathSelected = this.path;
    } else {
      console.log(data.data);
    }
  }

  loadPersonalFiles() {
    this.selectedItem = -1;
    this.personalFiles = true;
    this.path = 'MyFiles';
    this.pathSelected = this.path;
    WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({ username: this.user.username, path: this.path }));
  }

  loadSharedFiles() {
    this.selectedItem = -1;
    this.personalFiles = false;
    this.path = 'SharedFiles';
    this.pathSelected = this.path;
    WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({ username: this.user.username, path: this.path }));
  }

  goBack() {
    let files: string[] = this.path.split('/');
    if (files.length == 1) { return; }
    files.pop();
    this.path = files.join('/');
    this.pathSelected = this.path
    WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({ username: this.user.username, path: this.path }));
  }


  removeFile() {
    var r = confirm(`Are you sure?`);
    if (r == true) {
      let route = this.files[this.selectedItem].route + '/' + this.files[this.selectedItem].fileName;
      if (this.files[this.selectedItem].type == 'file') {
        route = route + '.' + this.files[this.selectedItem].FileExtension;
        WebSocketService.stompClient.send('/app/delete-file', {}, JSON.stringify({ username: this.user.username, path: route, pathFiles: this.path }));
      } else {
        WebSocketService.stompClient.send('/app/delete-folder', {}, JSON.stringify({ username: this.user.username, path: route, pathFiles: this.path }));
      }
    } else {
      return;
    }
  }


  changeFileSelected(index: number) {

    if($('.modal.show').length){return;}
    if (this.selectedItem == index) {
      this.selectedItem = -1;
      let files: string[] = this.pathSelected.split('/');
      if (files.length == 1) { return; }
      files.pop();
      this.pathSelected = files.join('/');
    }
    else {
      this.selectedItem = index
      if (this.files[this.selectedItem].type == 'folder') {
        this.pathSelected = this.files[this.selectedItem].route + '/' + this.files[this.selectedItem].fileName
      } else {
        this.pathSelected = this.files[this.selectedItem].route + '/' + this.files[this.selectedItem].fileName + '.' + this.files[this.selectedItem].FileExtension
      }
    }
    console.log(this.path)
    console.log(this.pathSelected)
  }


  openFile(file:File) {
    if (file.type == 'folder') {
      WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({username:this.user.username, path:file.route + '/'+ file.fileName}));
    }
    // type file
    else {
      this.fileToOpen = file;
      document.getElementById('btn-showText').click();
    }
  }

  

  saveChanges() {
    
    let newText = (<HTMLTextAreaElement>document.getElementById('text-id')).value;
    console.log("SE LLAMA")

    if(newText != this.fileToOpen.text) {
      // send to edit file
      let route = this.fileToOpen.route + '/' + this.fileToOpen.fileName +'.' + this.fileToOpen.FileExtension;
      WebSocketService.stompClient.send('/app/edit-file', {}, JSON.stringify({username:this.user.username, path:route, text:newText}));
    }
    document.getElementById('btn-close').click();
  }

}
