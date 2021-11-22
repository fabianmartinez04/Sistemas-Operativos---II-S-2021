import { Component, Input, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-create-folder',
  templateUrl: './create-folder.component.html',
  styleUrls: ['./create-folder.component.css']
})
export class CreateFolderComponent implements OnInit {

  @Input() username : string = '';
  @Input() route: string = '';

  file : File;
  constructor() { }

  ngOnInit(): void {
    this.file = new File();
    this.file.type = 'folder';
  }


  createFolder(form : NgForm) {
    if(form.invalid){return;}
    
    console.log(form);

    WebSocketService.stompClient.send('/app/create-folder', {}, JSON.stringify({name:this.file.fileName, username: this.username, path : this.route}));
  } 

}
