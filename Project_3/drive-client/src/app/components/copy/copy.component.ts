import { Component, Input, OnInit } from '@angular/core';

@Component({
  selector: 'app-copy',
  templateUrl: './copy.component.html',
  styleUrls: ['./copy.component.css']
})
export class CopyComponent implements OnInit {

  @Input() route: string = '';
  @Input() type: string= '';

  constructor() { }

  ngOnInit(): void {
  }

}
