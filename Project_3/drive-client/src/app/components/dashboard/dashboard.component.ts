import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { User } from 'src/app/models/user';
import { File } from 'src/app/models/file';

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrls: ['./dashboard.component.css']
})
export class DashboardComponent implements OnInit {

  user:User;
  files: File[] = [];
  personalFiles:boolean = true;
  selectedItem : number = -1;
  directory:string= 'My files';

  constructor(private router: Router, private activatedRouter : ActivatedRoute) { }

  ngOnInit(): void {
    this.user = new User();
    this.activatedRouter.params.subscribe((params:any)=> {
      this.user.username = params['username'];

      // call a service to get files of directory
    })
    this.files.push(new File());
    this.files.push(new File());
    this.files.push(new File());
    this.files.push(new File());
    this.files[0].test("Folder 1","folder");
    this.files[1].test("File 1","file");
    this.files[2].test("Folder 2","folder");    
    this.files[3].test("Folder 3","folder");
  }


  exit() {
    this.router.navigateByUrl('/user-login');
  }

  loadPersoonalFiles() {
    this.personalFiles = true;
    this.directory = 'My Files';
  }

  loadSharedFiles() {
    this.personalFiles = false;
    this.directory = 'Shared Files';
  }

  openFile() {
    console.log("this is dobule click");
  }

}