import { Component, Input, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';

import $ from 'jquery';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-copy-rv',
  templateUrl: './copy-rv.component.html',
  styleUrls: ['./copy-rv.component.css']
})
export class CopyRvComponent implements OnInit {

  @Input() path : string = '';
  @Input() username : string = '';

  file : File;

  constructor() { }

  ngOnInit(): void {
    this.file = new File();
    this.file.route = this.path;
    this.file.owner = this.username;
  }

  copy(form: NgForm) {
    if(form.invalid) { return; }

    var file = (<HTMLInputElement>document.getElementById('file-input-rv')).files[0];
    this.file.fileName = file.name;
    var Reader = new FileReader();
    if(this.file.type == 'file') {
      //this.file.type = file.type;
      this.file.type = 'txt';
      this.file.size = file.size;
      Reader.readAsText(file);
      Reader.onload =() => {
        let text = Reader.result;
        if(typeof text == typeof '') {
          this.file.text = String(text);
        }
        WebSocketService.stompClient.send('/app/create-file', {},JSON.stringify({username:this.username, text: this.file.text, name:this.file.fileName, path:this.file.route, extension:this.file.FileExtension}))
      };
    } else {
    
    }


    //call service
    form.resetForm();
    (<HTMLInputElement>document.getElementById('file-input-rv')).value = '';
    $('#cancel-btn-modal').click();

  }
  
  

}
