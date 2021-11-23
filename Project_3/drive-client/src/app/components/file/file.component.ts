import { Component, Input, OnInit } from '@angular/core';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

import $ from 'jquery';

@Component({
  selector: 'app-file',
  templateUrl: './file.component.html',
  styleUrls: ['./file.component.css']
})
export class FileComponent implements OnInit {

  @Input() file : File = new File();
  @Input() selected: boolean = false;
  @Input() username: string = '';

  isEdit : Boolean = false;

  constructor() { }

  ngOnInit(): void {
  }


  openFile() {
    if (this.file.type == 'folder') {
      WebSocketService.stompClient.send('/app/loadFiles', {}, JSON.stringify({username:this.username, path:this.file.route + '/'+ this.file.fileName}));
    }
    // type file
    else {
      document.getElementById('btn-showText').click();
    }
  }


  saveChanges() {
    let newText = (<HTMLParagraphElement>document.getElementById("text-id")).innerHTML;

    if(newText == this.file.text) {
      // send to edit file
      let route = this.file.route + '/' + this.file + '.' + this.file.FileExtension;
      WebSocketService.stompClient.send('/app/edit-file', {}, JSON.stringify({username:this.username, path:route, text:newText}));
    }
    document.getElementById('btn-close').click();
  }

}
