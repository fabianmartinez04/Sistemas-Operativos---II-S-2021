import { ComponentFixture, TestBed } from '@angular/core/testing';

import { CreateDriveComponent } from './create-drive.component';

describe('CreateDriveComponent', () => {
  let component: CreateDriveComponent;
  let fixture: ComponentFixture<CreateDriveComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ CreateDriveComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(CreateDriveComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
