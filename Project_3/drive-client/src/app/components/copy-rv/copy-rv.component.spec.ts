import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CopyRvComponent } from './copy-rv.component';

describe('CopyRvComponent', () => {
  let component: CopyRvComponent;
  let fixture: ComponentFixture<CopyRvComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ CopyRvComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(CopyRvComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
