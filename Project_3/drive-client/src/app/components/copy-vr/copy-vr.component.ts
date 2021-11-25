import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';

import $ from 'jquery'

@Component({
  selector: 'app-copy-vr',
  templateUrl: './copy-vr.component.html',
  styleUrls: ['./copy-vr.component.css']
})
export class CopyVrComponent implements OnInit {

  file : string = '';

  constructor() { }

  ngOnInit(): void {
  }

  
  copy(form: NgForm) {
    if(form.invalid) { return; }

    //call service
    console.log(this.file)
    form.resetForm();
    $('#cancel-btn').click();
  }
}
