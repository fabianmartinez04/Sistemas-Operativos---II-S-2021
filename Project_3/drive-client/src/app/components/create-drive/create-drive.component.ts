import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { Router } from '@angular/router';
import { User } from 'src/app/models/user';

@Component({
  selector: 'app-create-drive',
  templateUrl: './create-drive.component.html',
  styleUrls: ['./create-drive.component.css']
})
export class CreateDriveComponent implements OnInit {

  user:User;

  constructor(private router: Router) { }

  ngOnInit(): void {
    this.user = new User();
  }


  createDrive(form: NgForm) {
    if(form.invalid) { return; }

    this.router.navigateByUrl('/drive-dashboard');
  }

}
