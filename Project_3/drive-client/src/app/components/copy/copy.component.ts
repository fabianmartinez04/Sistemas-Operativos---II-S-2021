import { Component, Input, OnInit } from '@angular/core';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-copy',
  templateUrl: './copy.component.html',
  styleUrls: ['./copy.component.css']
})
export class CopyComponent implements OnInit {

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
      this.fileCopy = this.file;
      this.path = 'MyFiles';
      let body = JSON.parse(data.body);
      this.fileSystem = body.data;
      this.foldersQueue = [];
      this.foldersQueue.push(this.fileSystem);
      this.loadFolders(this.fileSystem);
    });
  }

  openFolders(folder:any){
    this.foldersQueue.push(folder);
    this.path = this.path + '/' + folder.name;
    this.loadFolders(folder);
  }


  loadFolders(file) {
    
    let children : [] = file.children;
    this.folders = [];
    children.forEach((element:any) => {
      if(element.type == 'folder' &&
        element.route + '/' + element.name != this.fileCopy.route + '/' + this.fileCopy.fileName) {
        this.folders.push(element);
      }
    });
    
  }

  copy() {
    let path: string = this.fileCopy.route + '/'+ this.fileCopy.fileName
    if (this.fileCopy.type == 'file') {
      path = path + '.' + this.fileCopy.FileExtension;
      WebSocketService.stompClient.send('/app/copy-file', {}, JSON.stringify({username:this.username, path:path, newPath:this.path}));
    } else {
      WebSocketService.stompClient.send('/app/copy-folder', {}, JSON.stringify({username:this.username, path:path, newPath:this.path}));
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

}
