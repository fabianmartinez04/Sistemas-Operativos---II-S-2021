import { Component, Input, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';

@Component({
  selector: 'app-shared',
  templateUrl: './shared.component.html',
  styleUrls: ['./shared.component.css']
})
export class SharedComponent implements OnInit {

  @Input() route : string = '';
  username : string = '';

  constructor() { }

  ngOnInit(): void {
  }


  shared(form: NgForm) {
    if(form.invalid) { return; }
    document.getElementById("cancel-btn").click();

    //call service
    console.log(this.username)
  }
}
