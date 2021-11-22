import { Component, Input, OnInit } from '@angular/core';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-file',
  templateUrl: './file.component.html',
  styleUrls: ['./file.component.css']
})
export class FileComponent implements OnInit {

  @Input() file : File = new File();
  @Input() selected: boolean = false;

  isEdit : Boolean = false;

  constructor() { }

  ngOnInit(): void {
  }


  openFile() {
    if (this.file.type == 'folder') {
      WebSocketService.stompClient.send();
    }
    // type file
    else {
      document.getElementById('btn-showText').click();
    }
  }


  saveChanges() {
    let newText = (<HTMLParagraphElement>document.getElementById("text-id")).innerHTML;
    console.log(newText);
  }

}
