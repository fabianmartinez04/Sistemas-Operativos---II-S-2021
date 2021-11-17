import { Component, Input, OnInit } from '@angular/core';
import { File } from 'src/app/models/file';

@Component({
  selector: 'app-file',
  templateUrl: './file.component.html',
  styleUrls: ['./file.component.css']
})
export class FileComponent implements OnInit {

  @Input() file : File = new File();
  @Input() selected: boolean = false;

  constructor() { }

  ngOnInit(): void {
  }

}
