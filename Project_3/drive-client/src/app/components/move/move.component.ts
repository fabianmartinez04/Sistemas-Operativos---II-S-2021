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
      let body = JSON.parse(data.body);
      this.fileSystem = body.data;
      this.foldersQueue = [];
      this.foldersQueue.push(this.fileSystem);
      this.loadFolders(this.fileSystem);
    });
  }

  loadFolders(file) {
    this.fileCopy = this.file;
    console.log(this.foldersQueue);
    let children : [] = file.children;
    this.folders = [];
    children.forEach((element:any) => {
      if(element.type == 'folder') {
        this.folders.push(element);
      }
    });
    
  }

  openFolders(folder:any){
    this.foldersQueue.push(folder);
    this.path = this.path + '/' + folder.name;
    console.log(this.foldersQueue)
    this.loadFolders(folder);
  }

  move() {
    let path: string = this.fileCopy.route + '/'+ this.fileCopy.fileName
    if (this.fileCopy.type == 'file') {
      path = path + '.' + this.fileCopy.FileExtension;
      WebSocketService.stompClient.send('/app/move-file', {}, JSON.stringify({username:this.username, path:path, newPath:this.path}));
    } else {
      WebSocketService.stompClient.send('/app/move-file', {}, JSON.stringify({username:this.username, path:path, newPath:this.path}));
    }
  }

  goBack() {
    if(this.foldersQueue.length == 1) { return; }
    let names: string[] = this.path.split('/');
    names.pop();
    this.path = names.join('/');
    this.foldersQueue.pop();
    console.log(this.foldersQueue)
    this.loadFolders(this.foldersQueue[this.foldersQueue.length - 1]);
    
  }

}
