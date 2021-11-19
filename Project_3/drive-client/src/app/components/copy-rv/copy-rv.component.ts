import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';

@Component({
  selector: 'app-copy-rv',
  templateUrl: './copy-rv.component.html',
  styleUrls: ['./copy-rv.component.css']
})
export class CopyRvComponent implements OnInit {

  file : string = '';

  constructor() { }

  ngOnInit(): void {
  }

  copy(form: NgForm) {
    if(form.invalid) { return; }
    document.getElementById("cancel-btn").click();

    //call service
    console.log(this.file)
  }
  
  

}
