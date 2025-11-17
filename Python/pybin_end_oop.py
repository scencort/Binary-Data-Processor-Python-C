import struct
from typing import List, Any

class Student:
    """
    Представляет одну запись о студенте\n
    Инкапсулирует данные и логику их форматирования для вывода
    """
    def __init__(self, student_id: int, gpa: float, surname: str, group: str):
        self.id = student_id
        self.gpa = gpa
        self.surname = surname
        self.group = group

    def __str__(self):
        """
        Метод для красивого строкового представления записи\n
        Позволяет легко вывести студента в консоль или записать в файл
        """

        return f"| {self.id:<4} | {self.surname:<15} | {self.gpa:<5.1f} | {self.group:<8} |"

class BinProcessor:
    def __init__(self, input_file: str, output_file: str, fmt: str):
        self.input_file = input_file
        self.output_file = output_file
        self.fmt = fmt
        self.record_size = struct.calcsize(fmt)
        self.students: List[Student] = []

    def read_unpack_data(self) -> bool:
        """
        Читает бинарный файл, распаковывает данные и создает объекты Student.
        """
        print(f"Чтение файла '{self.input_file}'. Размер записи: {self.record_size} байт.")

        with open(self.input_file, "rb") as file:
            while True:
                binary_data = file.read(self.record_size)

                if len(binary_data) < self.record_size:
                    break

                unpacked = struct.unpack(self.fmt, binary_data)

                surname = unpacked[2].decode('utf-8').strip('\x00').strip()
                group = unpacked[3].decode('utf-8').strip('\x00').strip()

                student = Student(unpacked[0], unpacked[1], surname, group)
                self.students.append(student)

        return True

    def write_data_to_txt(self):
        """
        Записывает список объектов Student в форматированную таблицу в текстовый файл
        """

        with open(self.output_file, "w", encoding='utf-8') as out_file:
            print(f"Запись данных в файл '{self.output_file}'")

            separator = "=" * 50

            out_file.write("Данные студентов:\n")
            out_file.write(separator + "\n")
            out_file.write(f"| {'ID':<4} | {'Фамилия':<15} | {'Балл':<5} | {'Группа':<8} |\n")
            out_file.write(separator + "\n")

            for student in self.students:
                out_file.write(str(student) + "\n")

            out_file.write(separator + "\n")

            print(f"Отсортированные данные сохранены в файле '{self.output_file}'")


FMT = "<Id16s8s"
INPUT_FILE = "../C/data.bin"
OUTPUT_FILE = "output.txt"

binarnik = BinProcessor(INPUT_FILE, OUTPUT_FILE, FMT)
binarnik.read_unpack_data()
binarnik.write_data_to_txt()