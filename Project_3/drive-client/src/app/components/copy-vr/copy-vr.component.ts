import { Component, Input, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';

import $ from 'jquery'
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-copy-vr',
  templateUrl: './copy-vr.component.html',
  styleUrls: ['./copy-vr.component.css']
})
export class CopyVrComponent implements OnInit {

  @Input() file : File;
  path : string = '';
  @Input() username : string = '';

  constructor() { }

  ngOnInit(): void {
  }

  
  copy(form: NgForm) {
    if(form.invalid) { return; }

    if(this.file.type == 'file') {
      let path = this.path + '/' + this.file.fileName + '.' + this.file.FileExtension;

      WebSocketService.stompClient.send('/app/copyVirtualToReal', {}, JSON.stringify({text:this.file.text, username:this.username, path: path}))
      form.resetForm();
      $('#cancel-btn-vr').click();
    }
  }
}
