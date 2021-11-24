import { Component, Input, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

import $ from 'jquery';

@Component({
  selector: 'app-create-folder',
  templateUrl: './create-folder.component.html',
  styleUrls: ['./create-folder.component.css']
})
export class CreateFolderComponent implements OnInit {

  @Input() username : string = '';
  @Input() route: string = '';
  @Input() files: File[] = [];

  file : File;
  constructor() { }

  ngOnInit(): void {
    this.file = new File();
    this.file.type = 'folder';
  }


  createFolder(form : NgForm) {
   
    if(form.invalid){return;}
    let createdFolder = true;

    this.files.forEach((element:File) => {
      if (element.type == 'folder' && element.fileName == this.file.fileName) {
        createdFolder = false;
        alert('Folder name already exist');
        
      }
    });
    if (createdFolder) {
      WebSocketService.stompClient.send('/app/create-folder', {}, JSON.stringify({name:this.file.fileName, username: this.username, path : this.route}));
      $('#btn-close').click();
    }
  } 

}
