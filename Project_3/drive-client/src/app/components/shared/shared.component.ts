import { Component, Input, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { File } from 'src/app/models/file';
import { WebSocketService } from 'src/app/services/web-socket.service';

@Component({
  selector: 'app-shared',
  templateUrl: './shared.component.html',
  styleUrls: ['./shared.component.css']
})
export class SharedComponent implements OnInit {

  @Input() file : File = new File();
  @Input() username: string = '';
  usertoshare : string = '';

  constructor() { }

  ngOnInit(): void {
  }


  shared(form: NgForm) {
    if(form.invalid) { return; }
    document.getElementById("cancel-btn").click();
    if(this.username != this.usertoshare){
      //call service
      console.log(this.username,this.usertoshare)
      if(this.file.type == "file"){
        WebSocketService.stompClient.send('/app/share-file', {}, JSON.stringify({username:this.username, usertoshare: this.usertoshare,path:this.file.route + '/'+ this.file.fileName + "." + this.file.FileExtension}));
      }
      else{
        WebSocketService.stompClient.send('/app/share-file', {}, JSON.stringify({username:this.username, usertoshare: this.usertoshare,path:this.file.route + '/'+ this.file.fileName}));
      }
     
    }
    else{
      alert("You should choose another username");
    }
   
  }
}
