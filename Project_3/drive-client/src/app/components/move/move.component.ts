import { Component, Input, OnInit } from '@angular/core';

@Component({
  selector: 'app-move',
  templateUrl: './move.component.html',
  styleUrls: ['./move.component.css']
})
export class MoveComponent implements OnInit {

  @Input() file: any;
  @Input() username : string = '';
  @Input() fileSystem: any = null;
  
  folders : any[] = [];


  constructor() { }

  ngOnInit(): void {
    // this.folders = this.fileSystem.MyFiles.children;
  }


  openFolders(folder){

  }

  move() {
    
  }

}
