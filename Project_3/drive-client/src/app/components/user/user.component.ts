import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { Router } from '@angular/router';
import { User } from 'src/app/models/user';

@Component({
  selector: 'app-user',
  templateUrl: './user.component.html',
  styleUrls: ['./user.component.css']
})
export class UserComponent implements OnInit {

  user:User;

  constructor( private router: Router) { }

  ngOnInit(): void {
    this.user = new User(); 
  }


  loadFiles(form: NgForm) {
    if(form.invalid) { return; }
    this.router.navigateByUrl(`/drive-dashboard/${this.user.username}`);
  }

}
