import { Component, Input, OnInit } from '@angular/core';

@Component({
  selector: 'app-move',
  templateUrl: './move.component.html',
  styleUrls: ['./move.component.css']
})
export class MoveComponent implements OnInit {

  @Input() route: string = '';
  @Input() type: string= '';
  @Input() username : string = '';
  @Input() FileSystem: any = null;

  
  constructor() { }

  ngOnInit(): void {
  }

}
