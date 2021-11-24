import { Component, Input, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { File } from 'src/app/models/file';

import $ from 'jquery';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-create-file',
  templateUrl: './create-file.component.html',
  styleUrls: ['./create-file.component.css']
})
export class CreateFileComponent implements OnInit {



  @Input() username : string = '';
  @Input() route: string = '';
  @Input() files : File[] = [];

  file:File;
  textTouched: boolean = true;
  constructor() { }

  ngOnInit(): void {
    this.file = new File();
    this.file.type = 'file';
    this.file.route = this.route;
  }

  createFile(form: NgForm) {
    if(form.invalid) { return; }
    let createdFile = true;
    this.file.text = (<HTMLTextAreaElement>document.getElementById('content-id')).value
    //call service
    if(this.file.text == '') {
      return;
    }
    this.files.forEach((element:File) => {
      if (element.fileName + '.' + element.FileExtension == this.file.fileName + '.' + this.file.FileExtension) {
        createdFile = false;
        alert('file name already exist');
        
      }
    });
    if (createdFile) {
      //WebSocketService.stompClient.send('/app/create-file',{}, JSON.stringify({username:this.username, path:this.file.route, name: this.file.fileName, extension: this.file.FileExtension, text:this.file.text}))

      WebSocketService.stompClient.send('/app/create-file',{}, JSON.stringify({username:this.username, path:this.route, name: this.file.fileName, extension: this.file.FileExtension, text:this.file.text}))

      this.file = new File();
      this.file.type = 'file';
      this.file.route = '';
      form.resetForm();
    }
    
  }

}
