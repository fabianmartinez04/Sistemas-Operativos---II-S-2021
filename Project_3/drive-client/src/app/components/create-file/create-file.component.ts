import { Component, Input, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { File } from 'src/app/models/file';

@Component({
  selector: 'app-create-file',
  templateUrl: './create-file.component.html',
  styleUrls: ['./create-file.component.css']
})
export class CreateFileComponent implements OnInit {

  @Input() username : string = '';
  @Input() route: string = '';

  file:File;
  constructor() { }

  ngOnInit(): void {
    this.file = new File();
    this.file.type = 'file';
  }

  createFile(form: NgForm) {
    if(form.invalid) { return; }
    document.getElementById("btn-close").click();

    //call service
    console.log(this.file)
  }

}
