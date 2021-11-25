import { Component, Input, OnInit } from '@angular/core';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-move',
  templateUrl: './move.component.html',
  styleUrls: ['./move.component.css']
})
export class MoveComponent implements OnInit {

  @Input() file: File;
  @Input() username : string = '';
  
  fileCopy : File;
  fileSystem: any = null;
  path : string = 'MyFiles';
  foldersQueue: any[] = [];
  
  folders : any[] = [];


  constructor() { }

  ngOnInit(): void {
    WebSocketService.stompClient.subscribe(`/queue/load-root-${this.username}`, (data:any) => {
      this.path = 'MyFiles';
      this.fileCopy = this.file;
      let body = JSON.parse(data.body);
      this.fileSystem = body.data;
      this.foldersQueue = [];
      this.foldersQueue.push(this.fileSystem);
      this.loadFolders(this.fileSystem);
    });
  }

  loadFolders(file) {
    let children : [] = file.children;
    this.folders = [];
    children.forEach((element:any) => {
      if(element.type == 'folder'&&
      element.route + '/' + element.name != this.fileCopy.route + '/' + this.fileCopy.fileName) {
        this.folders.push(element);
      }
    });
    
  }

  openFolders(folder:any){
    this.foldersQueue.push(folder);
    this.path = this.path + '/' + folder.name;
    this.loadFolders(folder);
  }

  move() {

    if(!this.checkFilesToDestination()){return;}

    let path: string = this.fileCopy.route + '/'+ this.fileCopy.fileName
    
    if (this.fileCopy.type == 'file') {
      let newPath = this.path;
      path = path + '.' + this.fileCopy.FileExtension;
      WebSocketService.stompClient.send('/app/move-file', {}, JSON.stringify({username:this.username, path:path, newPath: newPath, pathUpdate:this.fileCopy.route}));
    } else {
      WebSocketService.stompClient.send('/app/move-folder', {}, JSON.stringify({username:this.username, path:path, newPath:this.path, pathUpdate:this.fileCopy.route}));
    }
  }

  goBack() {
    if(this.foldersQueue.length == 1) { return; }
    let names: string[] = this.path.split('/');
    names.pop();
    this.path = names.join('/');
    this.foldersQueue.pop();
    this.loadFolders(this.foldersQueue[this.foldersQueue.length - 1]);
    
  }

  checkFilesToDestination() {
    let children : [] = this.foldersQueue[this.foldersQueue.length - 1].children;
    
    children.forEach((element:any) => {
      if (this.fileCopy.type == 'folder' && element.type == 'folder') {
        if (element.name == this.fileCopy.fileName) {
          alert('The folder to copy exist in destination path');
          return false;
        }
      } else if (this.fileCopy.type == 'file' && element.type == 'file') {
        if (element.name + '.' + element.extension == this.fileCopy.fileName + '.' + this.fileCopy.FileExtension) {
          alert('The file to copy exist in destinatioon path');
          return false;
        }
      }

    });
    
    return true;
  }

}
