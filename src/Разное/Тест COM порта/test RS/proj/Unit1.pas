unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, BCPort, StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    BComPort1: TBComPort;
    Memo1: TMemo;
    Memo2: TMemo;
    ComboBox1: TComboBox;
    Label1: TLabel;
    Edit1: TEdit;
    procedure Button3Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure BComPort1RxChar(Sender: TObject; Count: Integer);
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.Button3Click(Sender: TObject);
begin

  BComPort1.Port:=ComboBox1.Text;

  if (BComPort1.Port <> '') then begin
      try
        BComPort1.Open;
      except
        Memo2.Lines.Add('Error Open Serial port'); exit;
      end;
   end
   else begin
        Memo2.Lines.Add('Error Open Serial port'); exit;
   end;


end;

procedure TForm1.Button2Click(Sender: TObject);
begin

if (BComPort1.Connected) then begin

      BComPort1.Close;

 end;     
end;

procedure TForm1.Button1Click(Sender: TObject);
 var
        Oper: PAsync;
        u:String;

begin
    if (not BComPort1.Connected) then exit;

    u:=Edit1.Text;
    InitAsync(Oper);
    BComPort1.WriteAsync(u[1], Length(u), Oper);
    repeat
   	        Application.ProcessMessages ;
            sleep(20);
        Until
            BComPort1.IsAsyncCompleted(Oper);

            BComPort1.WaitForAsync(Oper);
            DoneAsync(Oper);
end;

procedure TForm1.BComPort1RxChar(Sender: TObject; Count: Integer);
var
        Str1:String;
        Count1,k,position,N:Integer;
        Stat:PAsync;

    begin
         Str1:='';
         Count1:=200;
         InitAsync(Stat);
         try
            BComPort1.ReadStrAsync(Str1, Count1, Stat);
         repeat
            Application.ProcessMessages ;
            sleep(200);
         until BComPort1.IsAsyncCompleted(Stat);
            N := BComPort1.WaitForAsync(Stat);

         if N <> Count1 then
            SetLength(Str1, N);
        finally
            DoneAsync(Stat);
        end;

        Memo1.Lines.Add(Str1);
end;

procedure TForm1.FormCreate(Sender: TObject);
begin

  ComboBox1.Clear;
  Memo1.Clear;
  Memo2.Clear;
  Edit1.Text:='';

  try
    EnumComPorts(ComboBox1.Items);
  except
    Memo2.Lines.Add('No Serial Ports'); exit;
  end;

end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin

  if (BComPort1.Connected) then begin

          BComPort1.Close;

  end;        

end;

end.
